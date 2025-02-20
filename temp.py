import os

def list_cc_files(folder_path):
    """
    Lists all .cc files in the specified folder.

    Args:
        folder_path (str): The path to the folder to search.

    Returns:
        list: A list of .cc file paths.
    """
    cc_files = []
    try:
        for file in os.listdir(folder_path):
            if file.endswith(".cc"):
                cc_files.append(os.path.join(folder_path, file))
    except FileNotFoundError:
        print(f"The folder '{folder_path}' does not exist.")
    except Exception as e:
        print(f"An error occurred: {e}")

    return cc_files

# Example usage:
if __name__ == "__main__":
    folder_path = input("Enter the folder path: ")
    cc_files = list_cc_files(folder_path)
    if cc_files:
        print("Found .cc files:")
        for file in cc_files:
            print("\""+file+"\",")
    else:
        print("No .cc files found.")
