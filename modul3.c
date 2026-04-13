#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>

// Fungsi blur
void apply_blur(unsigned char* in, unsigned char* out,
                int w, int h, int c, int k_size)
{
    int r = k_size / 2;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int ch = 0; ch < c; ch++) {

                float sum = 0;
                int count = 0;

                for (int ky = -r; ky <= r; ky++) {
                    for (int kx = -r; kx <= r; kx++) {

                        int px = x + kx;
                        int py = y + ky;

                        // Boundary check
                        if (px >= 0 && px < w && py >= 0 && py < h) {
                            sum += in[(py * w + px) * c + ch];
                            count++;
                        }
                    }
                }

                out[(y * w + x) * c + ch] =
                    (unsigned char)(sum / count);
            }
        }
    }
}

int main()
{
    int w, h, c;

    // Load gambar
    unsigned char* img = stbi_load("input.png", &w, &h, &c, 0);
    if (!img) {
        printf("Gagal load gambar\n");
        return 1;
    }

    // Alokasi output
    unsigned char* out = (unsigned char*)malloc(w * h * c);

    // Apply blur (kernel 3x3)
    apply_blur(img, out, w, h, c, 3);

    // Simpan hasil
    stbi_write_png("output.png", w, h, c, out, w * c);

    // Free memory
    stbi_image_free(img);
    free(out);

    printf("Selesai!\n");
    return 0;
}
