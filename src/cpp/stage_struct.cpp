#include "stage_struct.hpp"
#include <cstddef>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include "prim-types.hh"
#include "primvar.hh"
#include "tinyusdz.hh" // Include tinyusdz
#include "tydra/attribute-eval.hh"
#include "tydra/render-data.hh"
#include "tydra/scene-access.hh"
#include "tydra/shader-network.hh"
#include "pprinter.hh"
#include "prim-pprint.hh"

#include "rust/cxx.h"
#include "value-types.hh"
#include "xform.hh"

void get_all_prims(std::vector<tinyusdz::Prim>& out_prims, std::vector<tinyusdz::Prim> roots){
    for (auto root : roots){
        std::vector<tinyusdz::Prim> children;
        children = root.children();
        out_prims.insert(out_prims.end(), children.begin(), children.end());
        get_all_prims(out_prims, children);
    }
}

extern "C++" StageStruct load_file(rust::String file_path){
    StageStruct out;

    tinyusdz::Stage stage; // Stage in USD terminology is nearly meant for Scene in generic 3D graphics terminology.
    std::string warn;
    std::string err;


    // Auto detect USDA/USDC/USDZ
    std::string file_path_std = std::string(file_path);
    bool ret = tinyusdz::LoadUSDFromFile(file_path_std, &stage, &warn, &err);
    //Handle warings and errors
    if (warn.size()) {
      std::cout << "WARN : " << warn << "\n";
    }
    if (!ret) {
      if (!err.empty()) {
        std::cerr << "ERR : " << err << "\n";
      }
    }

    std::vector<tinyusdz::Prim> roots = stage.root_prims();
    std::cout << "n of roots : " << roots.size() << "\n";

    std::vector<tinyusdz::Prim> all_prims = roots;

    get_all_prims(all_prims, roots);

    std::cout << "n of prims : " << all_prims.size() << "\n";


    rust::Vec<PrimStruct> prim_structs;
    for (auto prim : all_prims){
        PrimStruct p;
        p.path = rust::String(prim.absolute_path().full_path_name());
        p.prim_type = rust::String(prim.type_name());
        std::cout << "PRIM path : " << p.path << "\n";



        // Compute and save world matrix for convenience TODO dot hsi properly, outputs local xform atm
        if (tinyusdz::IsXformablePrim(prim)) {
            const tinyusdz::Xformable *xform;
            tinyusdz::CastToXformable(prim, &xform);
            tinyusdz::value::matrix4d world_xform;
            std::string err;
            bool reset;
            xform -> EvaluateXformOps(0.0, tinyusdz::value::TimeSampleInterpolationType::Linear, &world_xform, &reset, &err);
            std::cout << "  --world xform " <<  world_xform << "\n";
            AttrStruct mtx;
            mtx.attr_name = "world_xform";
            mtx.attr_type = "matrix4d";
            mtx.attr_float.push_back(world_xform.m[0][0]);mtx.attr_float.push_back(world_xform.m[0][1]);mtx.attr_float.push_back(world_xform.m[0][2]);mtx.attr_float.push_back(world_xform.m[0][3]);
            mtx.attr_float.push_back(world_xform.m[1][0]);mtx.attr_float.push_back(world_xform.m[1][1]);mtx.attr_float.push_back(world_xform.m[1][2]);mtx.attr_float.push_back(world_xform.m[1][3]);
            mtx.attr_float.push_back(world_xform.m[2][0]);mtx.attr_float.push_back(world_xform.m[2][1]);mtx.attr_float.push_back(world_xform.m[2][2]);mtx.attr_float.push_back(world_xform.m[2][3]);
            mtx.attr_float.push_back(world_xform.m[3][0]);mtx.attr_float.push_back(world_xform.m[3][1]);mtx.attr_float.push_back(world_xform.m[3][2]);mtx.attr_float.push_back(world_xform.m[3][3]);
            p.attributes.push_back(mtx);
        }

        // Handle mesh specific primvars
        const tinyusdz::GeomMesh *mesh = prim.as<tinyusdz::GeomMesh>();
        if (mesh) {

            AttrStruct pts;
            pts.attr_name = "points";
            pts.attr_type = "point3f[]";
            std::vector<tinyusdz::value::point3f> pts_values = mesh->get_points();
            for (const auto& element : pts_values) {
                pts.attr_float.push_back(element.x);  // Back-insert into rust::Vec
                pts.attr_float.push_back(element.y);  // Back-insert into rust::Vec
                pts.attr_float.push_back(element.z);  // Back-insert into rust::Vec
            }
            p.attributes.push_back(pts);


            AttrStruct fvc;
            fvc.attr_name = "faceVertexCounts";
            fvc.attr_type = "int[]";
            std::vector<int> fvc_values = mesh->get_faceVertexCounts();
            for (const auto& element : fvc_values) {
                fvc.attr_int.push_back(element);  // Back-insert into rust::Vec
            }
            p.attributes.push_back(fvc);

            AttrStruct fvi;
            fvi.attr_name = "faceVertexIndices";
            fvi.attr_type = "int[]";
            std::vector<int> fvi_values = mesh->get_faceVertexIndices();
            for (const auto& element : fvi_values) {
                fvi.attr_int.push_back(element);  // Back-insert into rust::Vec
            }
            p.attributes.push_back(fvi);

        };

        std::vector<std::string> prop_names;
        std::string err;
        tinyusdz::tydra::GetPropertyNames(prim, &prop_names, &err);
        //std::cout << prop_names << "\n";
        for (auto prop_name : prop_names){
            tinyusdz::Property prop;
            tinyusdz::tydra::GetProperty(prim, prop_name, &prop, &err);
            std::cout << "  --prop " <<  prop_name << "\n";
            if (prop.is_attribute() && prop_name != "points" && prop_name != "faceVertexCounts" && prop_name != "faceVertexIndices"){ // skipping the primvars we get in geom mesh
                AttrStruct a;
                tinyusdz::Attribute attr = prop.get_attribute();
                //std::cout << "  --attr " << attr.type_name() << " - " <<  prop_name << "\n";
                a.attr_name = prop_name;
                a.attr_type = attr.type_name();

                // Simple Attributes
                if (attr.type_name() == "float"){
                    nonstd::optional<float> v = attr.get_value<float>();
                    if (v.has_value()){
                        std::cout << "      --float value " << v.value() << "\n";
                        a.attr_float.push_back(v.value());
                    }
                }
                else if (attr.type_name() == "bool"){
                    nonstd::optional<bool> v = attr.get_value<bool>();
                    if (v.has_value()){
                        std::cout << "      --bool value " << v.value() << "\n";
                        a.attr_bool.push_back(v.value());
                    }
                }
                // Primvar Attributes
                else if (attr.type_name() == "int[]"){
                    tinyusdz::primvar::PrimVar pv = attr.get_var();
                    if (pv.is_valid()){
                        std::vector<int> pvs = pv.get_value<std::vector<int>>().value_or(NULL);
                        if (!pvs.empty()){
                        std::cout << "      --primvar int[] len: " << pvs.size() << "\n";
                            for (auto v : pvs){
                                a.attr_int.push_back(v);
                            }
                        }
                    }
                }

                else if (attr.type_name() == "point3f[]"){
                    tinyusdz::primvar::PrimVar pv = attr.get_var();
                    if (pv.is_valid()){
                        std::vector<tinyusdz::value::point3f> pvs = pv.get_value<std::vector<tinyusdz::value::point3f>>().value_or(NULL);
                        if (!pvs.empty()){
                        std::cout << "      --primvar point3f[] len: " << pvs.size() << "\n";
                            for (auto v : pvs){
                                a.attr_float.push_back(v.x);
                                a.attr_float.push_back(v.y);
                                a.attr_float.push_back(v.z);
                            }
                        }
                    }
                }

                else if (attr.type_name() == "normal3f[]"){
                    tinyusdz::primvar::PrimVar pv = attr.get_var();
                    if (pv.is_valid()){
                        std::vector<tinyusdz::value::normal3f> pvs = pv.get_value<std::vector<tinyusdz::value::normal3f>>().value_or(NULL);
                        if (!pvs.empty()){
                        std::cout << "      --primvar normal3f len: " << pvs.size() << "\n";
                            for (auto v : pvs){
                                a.attr_float.push_back(v.x);
                                a.attr_float.push_back(v.y);
                                a.attr_float.push_back(v.z);
                            }
                        }
                    }
                }

                else if (attr.type_name() == "color3f[]"){
                    tinyusdz::primvar::PrimVar pv = attr.get_var();
                    if (pv.is_valid()){
                        std::vector<tinyusdz::value::color3f> pvs = pv.get_value<std::vector<tinyusdz::value::color3f>>().value_or(NULL);
                        if (!pvs.empty()){
                        std::cout << "      --primvar color3f len: " << pvs.size() << "\n";
                            for (auto v : pvs){
                                a.attr_float.push_back(v.r);
                                a.attr_float.push_back(v.g);
                                a.attr_float.push_back(v.b);
                            }
                        }
                    }
                }


                p.attributes.push_back(a);
            }

        }


        //std::cout << tinyusdz::print_prim_metas(prim.metas(), 1) << "\n";
        //std::cout << "n props : " << prim.metas().properties.size() << "\n";


        // Compute Xform of each Prim at time t.
        {
          tinyusdz::tydra::XformNode xformnode;
          double t = tinyusdz::value::TimeCode::Default();
          tinyusdz::value::TimeSampleInterpolationType tinterp = tinyusdz::value::TimeSampleInterpolationType::Held; // Held or Linear

          if (!tinyusdz::tydra::BuildXformNodeFromStage(stage, &xformnode, t, tinterp)) {
            std::cerr << "BuildXformNodeFromStage error.\n";
          } else {
            //std::cout << tinyusdz::tydra::DumpXformNode(xformnode) << "\n";
            //xformnode.get_world_matrix()
          }
        }


        out.prims.push_back(
            p
        );

    }

    // std::string h = stage.dump_prim_tree();//tinyusdz::to_string(stage);
    // std::cout << "USD : " << h << "\n";

    return out;
}
