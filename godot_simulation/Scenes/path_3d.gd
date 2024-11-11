extends Path3D
#
#var imu1_reading: Vector3
#var imu2_reading: Vector3

# the known control points
var a: Vector3
var d: Vector3

# the unknown control points
var b: Vector3
var c: Vector3


func rotation_matrix_from_rpy(roll: float, pitch: float, yaw: float) -> Transform3D:
	"""
	Take the roll, pitch and yaw angles measured by the IMU and returns a rotation matrix
	"""
	var cos_r = cos(roll)
	var sin_r = sin(roll)
	var cos_p = cos(pitch)
	var sin_p = sin(pitch)
	var cos_y = cos(yaw)
	var sin_y = sin(yaw)
	
	# Create rotation matrix (3x3 part of Transform3D)
	var rot_matrix = Transform3D()
	rot_matrix.basis[0] = Vector3(cos_y * cos_p, cos_y * sin_p * sin_r - sin_y * cos_r, cos_y * sin_p * cos_r + sin_y * sin_r)
	rot_matrix.basis[1] = Vector3(sin_y * cos_p, sin_y * sin_p * sin_r + cos_y * cos_r, sin_y * sin_p * cos_r - cos_y * sin_r)
	rot_matrix.basis[2] = Vector3(-sin_p, cos_p * sin_r, cos_p * cos_r)
	
	return rot_matrix

func get_tangent_vector(roll: float, pitch: float, yaw: float) -> Vector3:
	"""
	Take the roll, pitch and yaw angles measured by the IMU and returns a gradient vector dB/dt
	"""
	var rotation = rotation_matrix_from_rpy(roll, pitch, yaw)
	var base_direction = Vector3(1, 0, 0)  # Assuming the forward direction of the IMU is along the x-axis
	return rotation.basis * base_direction
	
func solve_for_b_and_c(a: Vector3, d: Vector3, b: Vector3, c: Vector3, tangent_t0_2: Vector3, tangent_t0_8: Vector3):
	# Coefficients from the Bézier derivative equation
	var coeff_t0_2_a = -1.92
	var coeff_t0_2_b = 0.96
	var coeff_t0_2_c = 0.84
	var coeff_t0_2_d = 0.12
	
	var coeff_t0_8_a = -0.12
	var coeff_t0_8_b = -0.84
	var coeff_t0_8_c = -0.96
	var coeff_t0_8_d = 1.92
	# TODO: Solve for b a
	# b = ?
	# c = ?
	
	print(b)
	print(c)
	
func _ready():
	
	# imu1_readings
	var roll1 = 0 # Example roll angle in radians
	var pitch1 = 0  # Example pitch angle in radians
	var yaw1 = 1 # Example yaw angle in radians

	#imu2_readings
	var roll2 = 0
	var pitch2 = 0
	var yaw2 = -1

	var a = Vector3(0, 2.5, 0)  # Example start point
	var d = Vector3(0, -2.5, 0)  # Example end point

	var tangent_t0_2 = get_tangent_vector(roll1, pitch1, yaw1)
	var tangent_t0_8 = get_tangent_vector(roll2, pitch2, yaw2)

	solve_for_b_and_c(a, d, b, c,tangent_t0_2, tangent_t0_8)

	print("Control Point b: ", b)
	print("Control Point c: ", c)
