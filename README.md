# Perlin Noise Texture Generator:

The Perlin Noise Texture Generator is a C++ program that generates random, yet smooth, textures using Perlin Noise. Perlin noise is widely used in computer graphics for generating textures, landscapes, and other natural-looking randomness.
This project demonstrates the creation of a 2D Perlin Noise texture and the ability to generate it as an image file. The generated textures can be used in various applications like procedural content generation, texture mapping, and simulations of natural phenomena.

## Iteration 4 : Improved Perlin Noise
Using circle sampling instead of square sampling, and a better smoothstep function.

<img src="https://github.com/user-attachments/assets/26444c8c-b60a-4153-a83a-2532c09d9ac6" alt="image" width="400">


## Iteration 3 : Original Perlin Noise
Perlin noise is now implemented in a better way, we define a grid, generate random vectors in the grid, dot product it with the center, and then interpolate with smoothstep for each pixel.

<img src="https://github.com/user-attachments/assets/26444c8c-b60a-4153-a83a-2532c09d9ac6" alt="image" width="400">



## Iteration 2 : Value Noise
Value noise is now implemented in a better way, we define a grid, generate random values in the grid, and then interpolate with smoothstep for each pixel.


<img src="https://github.com/user-attachments/assets/5bc79fb5-718c-4645-88d3-1e0fae31b06a" alt="image" width="400">



## Iteration 1 : White Noise
In the first iteration, the program generates completely random noise, which results in very chaotic, non-smooth textures. These textures are not yet useful for applications that require more controlled, smooth randomness, such as terrain generation or cloud textures.

<img src="https://github.com/user-attachments/assets/c31850eb-903c-4410-a48b-b1354030eccf" alt="image" width="400">


