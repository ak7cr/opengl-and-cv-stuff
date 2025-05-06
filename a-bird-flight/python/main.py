# The mathematical artwork “A Bird in Flight” by Hamid Naderi Yeganeh 
# can be generated using 2,000 line segments whose endpoints follow specific 
# trigonometric formulas American Mathematical Society . Each segment is defined by 
# two points 𝑃 𝑖 P i ​ and 𝑄 𝑖 Q i ​ in the plane for 𝑖 = 1 , … , 2000 i=1,…,2000, 
# creating an optical illusion of a bird in motion American Mathematical Society . 
# Below, you’ll find the exact formulas and a Python script using NumPy and Matplotlib 
# to render the image on your PC.
import numpy as np
import matplotlib.pyplot as plt

# Number of segments
N = 2000

# Parameter array
i = np.arange(1, N + 1)

# Compute endpoints P_i
x1 = 3 * np.sin(2 * np.pi * i / N) ** 3
y1 = -np.cos(8 * np.pi * i / N)

# Compute endpoints Q_i
x2 = (3/2) * np.sin(2 * np.pi * i / N) ** 3
y2 = -0.5 * np.cos(6 * np.pi * i / N)

# Plotting
plt.figure(figsize=(8, 6))
for xi1, yi1, xi2, yi2 in zip(x1, y1, x2, y2):
    plt.plot([xi1, xi2], [yi1, yi2], linewidth=0.5)

# Maintain aspect ratio and hide axes for a clean look
plt.axis('equal')
plt.axis('off')

# Display the image
plt.show()
