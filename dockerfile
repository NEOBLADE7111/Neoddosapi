FROM ubuntu:22.04

WORKDIR /app

# Install Python, GCC and tools
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    gcc \
    g++ \
    make \
    libc-dev \
    net-tools \
    curl \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Create symbolic link for python
RUN ln -s /usr/bin/python3 /usr/bin/python

COPY requirements.txt .
RUN pip3 install --no-cache-dir -r requirements.txt

COPY . .

RUN mkdir -p binaries c_files logs

RUN chmod +x binaries/* 2>/dev/null || true

EXPOSE 8080

CMD ["python3", "app.py"]