idf_component_register(
    SRC_DIRS "."          # 源文件就在当前目录
    INCLUDE_DIRS "."       # 头文件也在当前目录
    REQUIRES lvgl esp_lvgl_port # 声明依赖
)
