emcc npc_gen.cpp -std=c++11 -o npc_gen.html --bind -s WASM=1
rm npc_gen.html #we're using our own HTML file.
