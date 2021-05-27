package msd.edu.webChatServer;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.socket.WebSocketSession;

import java.util.HashMap;

@SpringBootApplication
public class WebChatServerApplication {
	public static HashMap<String, Room> rooms = new HashMap<>();
	public static HashMap<WebSocketSession, String> activeRooms = new HashMap<>();
	public static void main(String[] args) {
		SpringApplication.run(WebChatServerApplication.class, args);
	}
}
