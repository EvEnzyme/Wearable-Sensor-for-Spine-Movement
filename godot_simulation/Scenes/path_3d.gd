extends Path3D

signal connected # when connected to server
signal data # when data is received
signal disconnected # when disconnected from server
signal error # if an error occurs with the connection

var _status: int = 0 # status of stream
var _stream: StreamPeerTCP = StreamPeerTCP.new() # create new TCP stream
#
## attempt to connect
#func connect_to_host(host: String, port: int) -> void:
	#print("Connecting to %s:%d" % ["127.0.0.1", "10200"])
	## Reset status so we can tell if it changes to error again.
	#_status = _stream.STATUS_NONE
	#if _stream.connect_to_host(host, port) != OK:
		#print("Error connecting to host.")
		#emit_signal("error")
#
## attempt to send data to client
#func send(data: PackedByteArray) -> bool:
	#if _status != _stream.STATUS_CONNECTED:
		#print("Error: Stream is not currently connected.")
		#return false
	#var error: int = _stream.put_data(data)
	#if error != OK:
		#print("Error writing to stream: ", error)
		#return false
	#return true


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#_status = _stream.get_status()
	pass


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	#var new_status: int = _stream.get_status()
	## changing past event status to the current one + emitting associated signal for the previous status
	#if new_status != _status:
		#_status = new_status
	if _stream.connect_to_host("127.0.0.1", 10200) != OK:
		print("Error connecting to host.")
		emit_signal("error")
		
	#match _stream.get_status():
		#_stream.STATUS_NONE:
			#print("Disconnected from host.")
			#emit_signal("disconnected")
		#_stream.STATUS_CONNECTING:
			#print("Connecting to host.")
		#_stream.STATUS_CONNECTED:
			#print("Connected to host.")
			#emit_signal("connected")
		#_stream.STATUS_ERROR:
			#print("Error with socket stream.")
			#emit_signal("error")
	## if successfully connected, continuously read data from each individual event
	#if _status == _stream.STATUS_CONNECTED:
		#var available_bytes: int = _stream.get_available_bytes()
		#if available_bytes > 0:
			#print("available bytes: ", available_bytes)
			#var data: Array = _stream.get_partial_data(available_bytes)
			## Check for read error.
			#if data[0] != OK:
				#print("Error getting data from stream: ", data[0])
				#emit_signal("error")
			#else:
				#emit_signal("data", data[1])
