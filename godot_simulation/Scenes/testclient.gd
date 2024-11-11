extends Node

# File for testing/hosting TCP client

const HOST: String = "127.0.0.1"
const PORT: int = 10200

# Preload the client script
const Client = preload("res://Scenes/TCPconnection.gd")

# Variable to hold the client instance
var _client: Node = Client.new()  # Ensure this matches the class type in client.gd

func _ready() -> void:
	# Connect signals from the client
	_client.connect("connected", Callable(self, "_handle_client_connected"))
	_client.connect("disconnected", Callable(self, "_handle_client_disconnected"))
	_client.connect("error", Callable(self, "_handle_client_error"))
	_client.connect("data", Callable(self, "_handle_client_data"))
	
	add_child(_client)  # Add the client as a child node
	_client.connect_to_host(HOST, PORT)  # Connect to the host

# Handle events
func _handle_client_connected() -> void:
	print("Client connected to server.")

func _handle_client_data(data: PackedByteArray) -> void:
	print("Client data: ", data.get_string_from_utf8())
	var message: PackedByteArray = [97, 99, 107]  # Bytes for "ack" in ASCII
	_client.send(message)

func _handle_client_disconnected() -> void:
	print("Client disconnected from server.")

func _handle_client_error() -> void:
	print("Client error.")

func _process(delta: float) -> void:
	pass
