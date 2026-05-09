#include "web.h"

#ifdef __EMSCRIPTEN__
    #include <emscripten/emscripten.h>

    EM_JS(void, load_image_impl, (), {
        const input = document.createElement('input');

        input.type = 'file';
        input.accept = '.png,.jpg,.jpeg';

        input.onchange = function(event) {
            const file = event.target.files[0];

            file.arrayBuffer().then(function(buffer) {
                const data = new Uint8Array(buffer);

                const data_ptr = _malloc(data.length);
                HEAPU8.set(data, data_ptr);

                const name_len = lengthBytesUTF8(file.name) + 1;
                const name_ptr = _malloc(name_len);

                stringToUTF8(file.name, name_ptr, name_len);

                Module.ccall('image_loaded', null, ['number', 'number', 'number'], [data_ptr, data.length, name_ptr]);
                Module._free(data_ptr);
                Module._free(name_ptr);
            });
        };

        input.click();
    });

    void load_image() {
        load_image_impl();
    }
#endif