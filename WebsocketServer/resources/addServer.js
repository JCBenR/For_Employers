let num1, num2;

/*==========================WEBSOCKET VERSION====================== */

let myWS = new WebSocket('ws://localhost:8080');

myWS.onerror = function (event) {
    console.log("error has occured: " + event);
}

myWS.onopen = function () {
    console.log('now running websocket');
}

function wsOpen() {
    console.log('WebSocket sending data');
    num1 = document.getElementById('numOne').value;
    num2 = document.getElementById('numTwo').value;
    // console.log(num1 + " & " + num2);
    // myWS.onopen(`/calculate?x=${num1}&y=${num2}`);
    myWS.send(`${num1} ${num2}`);
}

myWS.onmessage = function (event) {
    console.log(event);
    let result = document.getElementById("responseAnswer");
    result.style.color = 'green';
    result.innerHTML = event.data;
}


/*==========================XHR VERSION====================== */
let xhr = new XMLHttpRequest();

function sendData() {
    console.log('sending data');
    num1 = document.getElementById('numOne').value;
    num2 = document.getElementById('numTwo').value;
    // console.log(num1 + " & " + num2);
    xhr.open('POST', `/calculate?x=${num1}&y=${num2}`)
    xhr.send();
}

xhr.onerror = function (event) {
    console.log("error: " + event);
}

xhr.onload = function () {
    let result = document.getElementById("responseAnswer");
    result.style.color = 'red';
    result.innerHTML = xhr.response;
    console.log(xhr.response)
    if (xhr.status != 200) { // analyze HTTP status of the response
        console.log(`Error ${xhr.status}: ${xhr.statusText}`); // e.g. 404: Not Found
    } else { // show the result
        console.log(`answer is: ${xhr.response.length} bytes`); // response is the server
    }
};
//xhr.open(POST, 'http://localhost:8080/calculate',)