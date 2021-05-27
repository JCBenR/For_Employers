/*
TODO
====
1. GET INFO FROM FORM (USERNAME & CHATROOM)
2. SETUP WEBSOCKET CONNECTION
    --JOIN ROOM
3. RECEIVE JSON
4. FORMAT JSON
    --FIND ALL MESSAGES THAT ARE FROM CURRENT USER AND FORMAT
5. DISPLAY JSON
6. CREATE WEBPAGE FOR CHATROOM
    --CREATE FORM/ENTRY BOX
    --CREATE METHODS TO SUBMIT MESSAGES
    --ON SUBMIT, DISPLAY THE NEW MESSAGE
7. SOME WAY TO STYLE CHAT HISTOR AREA SO THAT IT'S A SCROLLING FIELD BUT THE TYPE/SUBMIT WINDOW STAYS IN VIEW
*/

let testString200 = "Lorem ipsum dolor sit amet, consectetur adipisicing elit. Aperiam eum, quo reiciendis sunt ullam vitae voluptates? Eaque esse fugit impedit nam neque numquam placeat quae vitae? Accusantium aliquam architecto asperiores atque blanditiis cumque, ducimus ea earum est illo ipsum itaque laborum maxime minima molestiae, nesciunt odio officiis perspiciatis recusandae sit sunt suscipit, totam vel vero voluptatem voluptates voluptatum. Animi aspernatur cumque dicta dolorem, eos harum laborum, laudantium officiis omnis quam recusandae sint ullam voluptatum! Atque est exercitationem id nemo placeat vitae. Cum dolor eaque fugiat, magnam maiores nobis praesentium qui quo reprehenderit voluptas. Eligendi esse magni quas? Commodi consequatur eius eos, non nostrum quis rem soluta totam? Iusto minus nihil ut voluptates. Aut culpa cum, esse ipsam laborum maiores nulla porro, quasi quia reiciendis repellendus repudiandae similique temporibus tenetur totam vero voluptatum! Accusamus corporis repellendus sed! Asperiores dolores eius hic laboriosam, quasi suscipit voluptatum? Ab aliquid consequatur deleniti doloremque, enim error hic natus nisi officiis sit. Aperiam consequuntur, doloremque ducimus eligendi eveniet, facere inventore ipsam iure neque perspiciatis placeat quasi quisquam quod repellat repellendus rerum vero voluptate voluptates? Atque blanditiis eos facilis labore minus mollitia repudiandae vel voluptatem! Assumenda eveniet ex non placeat sapiente. Aut et iste perferendis possimus totam!"

let userName, chatRoom;

//establish websocket
let myWS = new WebSocket("ws://" + location.host +"/chat");

function joinChat() {
    //extract UN and chat room
    userName = document.getElementById('userName').value;
    chatRoom = document.getElementById('chatRoom').value;
    document.getElementById("enterChat").addEventListener("click", function (event) {
        console.log(userName + " " + chatRoom);
    });

    //run joinRoom function
    joinRoom();

    //innerHTML to change upon click. display UN and chatroom.
    document.body.innerHTML =
        `<h1>Welcome to chatroom ${chatRoom}. You are user ${userName}.</h1>` +
        "<table id='messageTable'></table>" +
        "<textarea name='messageCenter' id='messageCenter' cols='30' rows='10'></textarea>" +
        "<br><br>" +
        "<button type='submit' id='messageButton' onclick='postMessage()'>Post Message</button>";
}


myWS.onerror = function (event) {
    console.log("Error has occured: ");

    console.log(event);
}

myWS.onopen = function () {
    // myWS.send("hello hello hello");
    console.log('now running websocket');
}

//sends appropriate string to log in.
function joinRoom() {
    myWS.send(`join ${chatRoom}`)
}

function isOpen(myWS) { return myWS.readyState === myWS.OPEN }

//sends userName and userMessage in approved format and logs them.
function postMessage() {
    if(!isOpen(myWS)) return;
    let userMessage = document.getElementById("messageCenter").value;
    myWS.send(`${userName} ${userMessage}`);
    console.log(`User message sent from ${userName}: ${userMessage}`);
}

function printMessage(event) {
    let table = document.querySelector("table");
    let messageCenter = document.getElementById("messageCenter");
    //by setting to '-1' this will insert it to the last row on the table.
    let row = table.insertRow(-1);
    let cell = row.insertCell(0);
    console.log(event);
    let data = JSON.parse(event.data);
    //create a new date. this will be used to timestamp (NOT WORKING)
    let today = new Date;
    //if post is from the current user, bold it. else, if it's from another user, don't.
    if (data.user == userName) {
        cell.innerHTML = `<strong>${data.user} @ ${today.toUTCString()}: ${data.message}</strong>`;
    } else if (data.user != userName) {
        cell.innerHTML = `${data.user} @ ${today.toUTCString()}: ${data.message}`;
    }
    //scroll to last row in table to keep most current message in view
    table.querySelector('tbody tr:last-child').scrollIntoView();
    //clear message box to empty for new typing
    messageCenter.value = "";
    console.log("message should be printed");
}

//on receiving a response, run printMessage function
myWS.onmessage = function (event) {
    console.log(event.data);
    printMessage(event);
}
