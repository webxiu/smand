"""把 resources/logo/logo@5x.png（256×256 唯一源图）缩放打包成多尺寸
resources/app.ico（PNG-in-ICO，Vista+ 标准）。

用法：py scripts/make_ico.py
更换 Logo 后只需同名替换 logo@5x.png 再重跑本脚本即可刷新 exe 文件图标；
其余尺寸（16/32/48/64/128）全部由 256 源图缩放生成，不必再维护多倍图。
"""
import io
import struct
import sys
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
BASE = ROOT / "resources" / "logo"
SRC = BASE / "logo@5x.png"  # 256×256 唯一源图，其余尺寸由它缩放生成
OUT = ROOT / "resources" / "app.ico"

# app.ico 内含的目标尺寸：256 直接用源图，其余由源图缩放补齐
SIZES = [16, 32, 48, 64, 128, 256]


def main() -> int:
    if not SRC.exists():
        print("[错误] 缺少源图:", SRC)
        return 1

    src_img = Image.open(SRC).convert("RGBA")

    blobs = []
    for size in SIZES:
        if size == src_img.width:
            # 与源图同尺寸：直接用原始 PNG 字节，避免二次编码损失
            data = SRC.read_bytes()
        else:
            img = src_img.resize((size, size), Image.Resampling.LANCZOS)
            buf = io.BytesIO()
            img.save(buf, format="PNG")
            data = buf.getvalue()
        blobs.append((size, data))

    count = len(blobs)
    header = struct.pack("<HHH", 0, 1, count)
    offset = 6 + 16 * count
    entries = b""
    payload = b""
    for size, data in blobs:
        dim = 0 if size >= 256 else size  # ICO 规范：256 记为 0
        entries += struct.pack("<BBBBHHII", dim, dim, 0, 0, 1, 32, len(data), offset)
        payload += data
        offset += len(data)

    OUT.write_bytes(header + entries + payload)
    print("written:", OUT, OUT.stat().st_size, "bytes,", count, "sizes")

    with Image.open(OUT) as ico:
        print("parsed sizes:", sorted(ico.info.get("sizes", [])))
    return 0


if __name__ == "__main__":
    sys.exit(main())
