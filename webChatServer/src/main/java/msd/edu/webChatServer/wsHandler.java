package msd.edu.webChatServer;

import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;
import com.google.gson.Gson;


import java.io.IOException;
import java.util.HashMap;

public class wsHandler extends TextWebSocketHandler {

    private String userName, roomName;
    private Room currentRoom = new Room();

    @Override
    public void handleTextMessage(WebSocketSession session, TextMessage message) throws IOException {
        //make string out of payload
        String payload = message.getPayload();
        //break payload into fields
        String[] getFields = payload.split(" ", 2);
        //assign to variables
        userName = getFields[0];
        roomName = getFields[1];
        if(!session.isOpen()){
            session.close();
            currentRoom.removeUser(session);}
        if(userName.equals("join")){
            Room r = currentRoom.findRoom(this);
            r.addClient(session);
            r.getMessageHistory(session);
            currentRoom = r;
        } else {
            currentRoom.postMessage(new TextMessage(makeJson(message)));

            currentRoom.messages.add(makeJson(message));
        }
    }

    public String getRoomName(){
        return roomName;
    }

    public static String makeJson(TextMessage message){
        String stringMsg = message.getPayload();
        String[] stringMsgArray = stringMsg.split(" ", 2);
        HashMap<String, String> msgMap = new HashMap<>();
        msgMap.put("user", stringMsgArray[0]);
        msgMap.put("message", stringMsgArray[1]);
        String finalString = new Gson().toJson(msgMap);
        System.out.println(finalString);
        return finalString;
    }

}
