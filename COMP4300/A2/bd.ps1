meson setup build-debug --backend vs --buildtype=debug
meson compile -C build-debug
meson install -C build-debug --destdir install 
copy ./build-debug/install/bin/* ./build-debug/ -Recurse -Force
copy ./data/* ./build-debug/ -Recurse -Force
