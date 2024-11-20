extends Path3D

# Member variables (optional, if you want to keep them globally accessible)
var imu1_position: Vector3
var imu2_position: Vector3

func _ready():
	# Initialize variables within the _ready function

	imu1_position = curve.get_point_position(0) # Position of the first IMU
	imu2_position = curve.get_point_position(10) # Position of the second IMU (10 units along z-axis)

	
	var rotation1 = get_rotation_matrix(0, 2, 4)
	var rotation2 = get_rotation_matrix(0, -2, 3)
	
	imu1_position = rotation1*imu1_position
	imu2_position = rotation2*imu2_position

	# Add the points to the curve
	curve.add_point(imu1_position)
	curve.add_point(imu2_position)
	
func get_rotation_matrix(roll: float, pitch: float, yaw: float)->Basis:
	var cx = cos(roll)
	var sx = sin(roll)
	var cy = cos(pitch)
	var sy = sin(pitch)
	var cz = cos(yaw)
	var sz = sin(yaw)
	
	return Basis(Vector3(cy * cz, cz * sx * sy - cx * sz, cx * cz * sy + sx * sz),
				 Vector3(cy * sz, cx * cz + sx * sy * sz, -cz * sx + cx * sy * sz),
				 Vector3(-sy, cy * sx, cx * cy))
