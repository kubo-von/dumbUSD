use cxx_build;

fn main() {
    //let out_dir = env::var("OUT_DIR").unwrap(); // Output directory for the build

    println!("cargo:rustc-link-lib=static=stage_struct");
    println!("cargo:rustc-link-search=native=src/cpp");
    println!("cargo:rustc-link-lib=static=tinyusdz");
    println!("cargo:rustc-link-search=native=src/cpp/include/tinyusdz/src");
    println!("cargo:rerun-if-changed=src/cpp/stage_struct.cpp");

    let files = [
        "src/cpp/include/tinyusdz/src/performance.cc",
        "src/cpp/include/tinyusdz/src/crate-writer.cc",
        "src/cpp/include/tinyusdz/src/composition.cc",
        "src/cpp/include/tinyusdz/src/usdShade.cc",
        "src/cpp/include/tinyusdz/src/xform.cc",
        "src/cpp/include/tinyusdz/src/stage.cc",
        "src/cpp/include/tinyusdz/src/tinyusdz.cc",
        "src/cpp/include/tinyusdz/src/ascii-parser-timesamples-array.cc",
        "src/cpp/include/tinyusdz/src/crate-pprint.cc",
        "src/cpp/include/tinyusdz/src/crate-reader.cc",
        "src/cpp/include/tinyusdz/src/usdFbx.cc",
        "src/cpp/include/tinyusdz/src/usda-reader.cc",
        "src/cpp/include/tinyusdz/src/io-util.cc",
        "src/cpp/include/tinyusdz/src/usd-to-json.cc",
        "src/cpp/include/tinyusdz/src/value-pprint.cc",
        "src/cpp/include/tinyusdz/src/str-util.cc",
        "src/cpp/include/tinyusdz/src/pprinter.cc",
        "src/cpp/include/tinyusdz/src/usdVox.cc",
        "src/cpp/include/tinyusdz/src/usdGeom.cc",
        "src/cpp/include/tinyusdz/src/prim-types.cc",
        "src/cpp/include/tinyusdz/src/image-writer.cc",
        "src/cpp/include/tinyusdz/src/usdLux.cc",
        "src/cpp/include/tinyusdz/src/usda-writer.cc",
        "src/cpp/include/tinyusdz/src/path-util.cc",
        "src/cpp/include/tinyusdz/src/usdMtlx.cc",
        "src/cpp/include/tinyusdz/src/primvar.cc",
        "src/cpp/include/tinyusdz/src/ascii-parser-basetype.cc",
        "src/cpp/include/tinyusdz/src/prim-composition.cc",
        "src/cpp/include/tinyusdz/src/prim-reconstruct.cc",
        "src/cpp/include/tinyusdz/src/ascii-parser.cc",
        "src/cpp/include/tinyusdz/src/usdc-reader.cc",
        "src/cpp/include/tinyusdz/src/ascii-parser-timesamples.cc",
        "src/cpp/include/tinyusdz/src/asset-resolution.cc",
        "src/cpp/include/tinyusdz/src/lz4-compression.cc",
        "src/cpp/include/tinyusdz/src/tiny-format.cc",
        "src/cpp/include/tinyusdz/src/usdObj.cc",
        "src/cpp/include/tinyusdz/src/crate-format.cc",
        "src/cpp/include/tinyusdz/src/linear-algebra.cc",
        "src/cpp/include/tinyusdz/src/usdc-writer.cc",
        "src/cpp/include/tinyusdz/src/json-to-usd.cc",
        "src/cpp/include/tinyusdz/src/value-types.cc",
        "src/cpp/include/tinyusdz/src/lz4/lz4.c",
        "src/cpp/include/tinyusdz/src/integerCoding.cpp",
        "src/cpp/include/tinyusdz/src/tydra/usd-export.cc",
        "src/cpp/include/tinyusdz/src/tydra/attribute-eval-typed-fallback.cc",
        "src/cpp/include/tinyusdz/src/tydra/attribute-eval-typed.cc",
        "src/cpp/include/tinyusdz/src/tydra/scene-access.cc",
        "src/cpp/include/tinyusdz/src/tydra/attribute-eval-typed-animatable-fallback.cc",
        "src/cpp/include/tinyusdz/src/tydra/obj-export.cc",
        "src/cpp/include/tinyusdz/src/tydra/attribute-eval.cc",
        "src/cpp/include/tinyusdz/src/tydra/texture-util.cc",
        "src/cpp/include/tinyusdz/src/tydra/facial.cc",
        "src/cpp/include/tinyusdz/src/tydra/shader-network.cc",
        "src/cpp/include/tinyusdz/src/tydra/attribute-eval-typed-animatable.cc",
        "src/cpp/include/tinyusdz/src/tydra/render-data.cc",
        "src/cpp/include/tinyusdz/src/tydra/json-export.cc",
        "src/cpp/include/tinyusdz/src/tydra/prim-apply.cc",
    ];

    // =X= disable this for faster builds when developing
    cxx_build::bridge("src/lib.rs")
        .files(files)
        .include("src/cpp") // Include directory for C++ headers
        .include("src/cpp/include/tinyusdz/src") // Include directory for C++ headers
        .include("src/cpp/include/tinyusdz/src/lz4") // Include directory for C++ headers
        .include("src/cpp/include/tinyusdz/src/nonstd") // Include directory for C++ headers
        .include("src/cpp/include/tinyusdz/src/tydra") // Include directory for C++ headers
        .flag_if_supported("-std=c++17") // Specify C++ standard if needed
        .warnings(false)
        .compile("tinyusdz");
    // =X=

    cxx_build::bridge("src/lib.rs")
        .file("src/cpp/stage_struct.cpp") // Path to your C++ source file
        .include("src/cpp") // Include directory for C++ headers
        .include("src/cpp/include/tinyusdz/src") // Include directory for C++ headers
        .include("src/cpp/include/tinyusdz/src/lz4") // Include directory for C++ headers
        .include("src/cpp/include/tinyusdz/src/nonstd") // Include directory for C++ headers
        .include("src/cpp/include/tinyusdz/src/tydra") // Include directory for C++ headers
        .include("src/cpp") // Include directory for C++ headers
        .flag_if_supported("-std=c++17") // Specify C++ standard if needed
        .warnings(false) // Suppress warnings if necessary
        .compile("stage_struct"); // Output library name (libmy_struct.a)

    // Link tinyusdz library
    println!("cargo:rustc-link-lib=static=tinyusdz"); // Use 'static' for static library
    println!("cargo:rustc-link-search=native=src/cpp/include/tinyusdz/src"); // Path to tinyusdz library
}
