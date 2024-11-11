# Wearable-Sensor-for-Spine-Movement

### Roll, Pitch, Yaw
<img title='row pitch yaw on a airplane' src='https://www1.grc.nasa.gov/wp-content/uploads/rotations.jpg' width=300px>
<img title='row pitch yaw in relation to the xyz axis>' src='https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcT-3sSEV0A_zFPNYYn4BraJ68UqKO_Z_E8Kbw&s' height=192px>


## Simulating the Spine Curve in Godot

### 0. Some Background Knowledge -- Bezier Curve

- **What it is:** Parametric Curves defined by a set of **control points**
- **How the curves are drawn:**
    - 3 control points in 2D:

        <img src = "https://upload.wikimedia.org/wikipedia/commons/3/3d/B%C3%A9zier_2_big.gif">

    - 4 control points in 2D:

        <img src = "https://upload.wikimedia.org/wikipedia/commons/d/db/B%C3%A9zier_3_big.gif">

- **Mathematical Definition:**

    - The general formula of Bézier curve $ \mathbf{B}(t) $ is given by:

        $$
        \mathbf{B}(t) = \sum_{i=0}^{n} \binom{n}{i} (1 - t)^{n-i} t^i \mathbf{P}_i
        $$

    - This can be expanded as:

        $$
        \mathbf{B}(t) = (1 - t)^n \mathbf{P}_0 + \binom{n}{1} (1 - t)^{n-1} t \mathbf{P}_1 + \cdots + \binom{n}{n-1} (1 - t) t^{n-1} \mathbf{P}_{n-1} + t^n \mathbf{P}_n, \quad 0 \leq t \leq 1
        $$

    - The derivative:
        $$
        \mathbf{B}'(t) = n \sum_{i=0}^{n-1} b_{i,n-1}(t) (\mathbf{P}_{i+1} - \mathbf{P}_i)
        $$

### 1. Defining Our Problem Space

-We aim to simulate the curvature of the spine based on IMU readings in Godot.


- For simplification, we treat the selected spine segment as a curved path defined by four control points: `A`, `B`, `C`, and `D`, with IMUs placed at positions corresponding to 20% and 80% along the path.

    <img src="images/simulation_problem_space.jpg">

- The IMUs' readings give us *some information* about the curve but not the location of the control points. 
- We define the start and end points of the path, `A` and `D`, which gives us two points to start with. Then, we work backwards using properties derived from the IMU readings to determine the coordinates of the intermediate control points `B` and `C`. 

    - Given: 
        1. The IMU readings `roll, pitch, yaw` at two positions along the curve.
        2. The distances or relative positions of the IMUs (20% and 80% along the curve).
        3. The coordinates of the start and end points, `A` and `D`.
    - To find: The coordinates of the intermediate control points `B` and `C`.

## 2. Solving the Problem

- For a 4-point Bezier Curve with points, $ \mathbf n=4 $, the equation with respect to the four points `A`, `B`, `C`,`D` is given by:

    $$
    \mathbf{B}(t) = (1-t)^3\mathbf{A}+3(1-t)^2t\mathbf{B}+3(1-t)t^2\mathbf{C}+t^3\mathbf{D},\ 0 \le t \le 1
    $$

    with derivative:

    $$
    \frac{d\mathbf{B}}{dt} = -3(1 - t)^2 \mathbf{A} + 3(1 - t)(1 - 3t) \mathbf{B} + 3t(2 - 3t) \mathbf{C} + 3t^2 \mathbf{D}
    $$

- Here, `A` and `D` are known, and we can get $ \frac{d\mathbf{B}}{dt} $ at $ \mathbf t=0.2, t=0.8 $ through the row, pitch, yaw angles provided by the two IMUs.

- Then, the only unkowns will by `B`, and `C`, which we can obtain by solving the two quadratic equations at $ \mathbf t=0.2, t=0.8 $

