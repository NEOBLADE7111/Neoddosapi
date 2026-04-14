import subprocess
import os
import glob

def compile_c_file(c_file_path):
    """Compile a C file to binary"""
    if not os.path.exists(c_file_path):
        return False, "File not found"
    
    if not c_file_path.endswith('.c'):
        return False, "Not a C file"
    
    binary_name = os.path.basename(c_file_path).replace('.c', '')
    binary_path = f"binaries/{binary_name}"
    
    try:
        cmd = ["gcc", c_file_path, "-o", binary_path, "-lpthread", "-O2"]
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        if result.returncode == 0:
            os.chmod(binary_path, 0o755)
            return True, f"Compiled: {binary_path}"
        else:
            return False, result.stderr
    except Exception as e:
        return False, str(e)

def compile_all_c_files():
    """Compile all .c files in current directory"""
    results = []
    for c_file in glob.glob("*.c"):
        if os.path.isfile(c_file):
            success, msg = compile_c_file(c_file)
            results.append({"file": c_file, "success": success, "message": msg})
    return results

if __name__ == "__main__":
    print("Compiling C files...")
    results = compile_all_c_files()
    for r in results:
        status = "✅" if r["success"] else "❌"
        print(f"{status} {r['file']}: {r['message']}")