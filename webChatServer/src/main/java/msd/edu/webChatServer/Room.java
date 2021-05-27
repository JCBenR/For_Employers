package msd.edu.webChatServer;

import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;

import java.io.IOException;
import java.util.ArrayList;

public class Room {
    public ArrayList<WebSocketSession> users = new ArrayList<>();
    public ArrayList<String> messages = new ArrayList<>();

    Room(){

    }

    synchronized public void addClient(WebSocketSession session) throws IOException {
        users.add(session);
    }

    synchronized public Room findRoom(wsHandler wr){
        String roomToLookFor = wr.getRoomName();
        if (WebChatServerApplication.rooms.containsKey(roomToLookFor)){
            System.out.println("now ENTERING room: "+ roomToLookFor);
            //something is happening here where it isn't saving
            Room aRoom = WebChatServerApplication.rooms.get(roomToLookFor);
            System.out.println("messages for room are"+aRoom.messages);
            return WebChatServerApplication.rooms.get(roomToLookFor);
        }else{
            Room newRoom = new Room();
            WebChatServerApplication.rooms.put(roomToLookFor, newRoom);
            System.out.println("now CREATING room: "+ roomToLookFor);
            return newRoom;
        }
    }

    synchronized public void getMessageHistory(WebSocketSession session) throws IOException {
        System.out.println("now getting message history");
        System.out.println("---------------------------");
        for (String m :
                messages) {
            System.out.println(m);
        }
        System.out.println("---------------------------");
        System.out.println(messages.size());
        for (String m :
                messages) {
            session.sendMessage(new TextMessage(m));
        }
    }

    synchronized public void removeUser(WebSocketSession session) throws IOException {
        users.remove(session);
    }

    public void postMessage(TextMessage message) throws IOException {
        for (WebSocketSession session: users
             ) {
            if(!session.isOpen()){
                session.close();
            } else session.sendMessage(message);
        }
    }
}
