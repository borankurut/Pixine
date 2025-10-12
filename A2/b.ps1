meson setup build --backend ninja --buildtype=release
meson compile -C build
meson install -C build --destdir install 
copy ./build/install/bin/* ./build/ -Recurse -Force
copy ./data/* ./build/ -Recurse -Force

