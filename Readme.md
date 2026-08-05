medialoader 是从love中提取出来的可以读取各种格式文件的库，主要是方便其他项目使用

## image-libs

这里实现了以下一些纹理格式的读取：
- DDS
- EXR
- KTX
- PNG
- ASTC

## sound-libs

这里提供了如下格式的音频读取：
- FLAC
- GME
- mp3
- wav

## model（assimp）

通过 vendored Assimp（`model-libs/assimp`）读取 3D 模型，CMake 目标：`medialoader_model`。

- `FileSystem` / `StdFileSystem`：可插拔 IO（支持 seek）；默认 C `FILE*`
- `ModelLoader::loadFromPath` / `loadFromMemory`：经 Assimp `IOSystem` 读入
- 返回 `ModelScene`（RAII，内部持有 `Assimp::Importer`），通过 `const aiScene*` 使用原生 Assimp 场景结构
- 头文件：`include/medialoader/model/`

本地测试：配置后构建并运行 `medialoader_model_test`。

## 计划实现的其他资源

1. 提供视频的读取

大资源往往需要可 seek 的文件系统抽象；模型侧已提供 `FileSystem` 接口，视频可复用同一套思路。
