FROM node:22-bookworm

RUN apt-get update \
    && apt-get install -y --no-install-recommends g++ python3 python3-venv \
    && rm -rf /var/lib/apt/lists/* \
    && npm install -g probhub@0.7.1 \
    && python3 -m venv /opt/probhub-python \
    && /opt/probhub-python/bin/python -m pip install --no-cache-dir \
       -r /usr/local/lib/node_modules/probhub/requirements.lock \
    && useradd --uid 10001 --create-home reviewer

ENV PATH="/opt/probhub-python/bin:${PATH}" \
    PYTHON="/opt/probhub-python/bin/python" \
    PYTHONUTF8=1 \
    PYTHONDONTWRITEBYTECODE=1 \
    HOME="/tmp"

COPY contribution.py container_entry.py smoke.py /opt/review/
USER 10001:10001
WORKDIR /tmp
ENTRYPOINT ["python", "/opt/review/container_entry.py"]
