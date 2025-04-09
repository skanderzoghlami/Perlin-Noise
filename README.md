# Perlin Noise Texture Generator:

The Perlin Noise Texture Generator is a C++ program that generates random, yet smooth, textures using Perlin Noise. Perlin noise is widely used in computer graphics for generating textures, landscapes, and other natural-looking randomness.
This project demonstrates the creation of a 2D Perlin Noise texture and the ability to generate it as an image file. The generated textures can be used in various applications like procedural content generation, texture mapping, and simulations of natural phenomena.

## Iteration 2 : Value Noise
Now instead of using just random values between [0,1] for each pixel, we implement Value noise by going trhough each pixel, hashing the 4 values that defines its box and then lerping between them with smoothstep for less linearity.

![output_video (3)](https://github.com/user-attachments/assets/e5bf2fdc-2802-4d6a-aaf0-09e27379efb1)


## Iteration 1 : White Noise
In the first iteration, the program generates completely random noise, which results in very chaotic, non-smooth textures. These textures are not yet useful for applications that require more controlled, smooth randomness, such as terrain generation or cloud textures.

![output_video (1)](https://github.com/user-attachments/assets/ced9a0ee-5230-41e0-9d60-e04e9fa2298e)


