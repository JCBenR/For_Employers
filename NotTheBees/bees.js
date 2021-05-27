//draw canvas
let canvas = document.getElementById("myCanvas");

//style canvas
canvas.style.border = "1px solid red";
let ctx = canvas.getContext("2d");
//load images and size
let tc = new Image();
tc.src = 'tcPNG.png';
tc.id = "tcImage";
let boom = new Image();
boom.src = 'explosionPNG.png';
boom.id = "boomImage";

//define and set mouse location
let currentX, currentY;
let mouseLocation = function (e) {
    currentX = e.offsetX;
    currentY = e.offsetY;
}

//define mouse move event listener
window.onload = function () {
    this.addEventListener('mousemove', mouseLocation);
}

//define arrays for holding X&Y and array for boom image objecs
//this will be referenced in positioning loop
let boomX = [];
let boomY = [];
let booms = [];

//define random variables to X&Y arrays. will be used to position the boom elements when drawn
for (let i = 0; i < 3; i++) {
    boomX.push(Math.floor((Math.random() * canvas.width) + 1));
    boomY.push(Math.floor((Math.random() * canvas.width) + 1));
}

//update position function. this basically looks at the current location of mouse (whihc is the tc object), and then can move the X&Y accordingly. if the boomX[i] is less than the currentX (mouse), that means it's to the left of the mouse and so needs to move right. likewise for Y axis, if the curretY is greater, than the boomY needs to move down.

function updateLocation(i, v) {
    if (currentX > boomX[i]) {
        boomX[i] += 3 + v;
    } else {
        boomX[i] -= 3 + v;
    }
    if (currentY > boomY[i]) {
        boomY[i] += 3 + v;
    } else {
        boomY[i] -= 3 + v;
    }
}

//this functions clears the canvas, loops through and draws the boom elements and updates their position, draws the tc object.
function callback() {
    ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height)
    //tc might need to go below the loop

    let step = 0
    for (let i = 0; i < 3; i++) {
        ctx.drawImage(boom, boomX[i], boomY[i], 100, 150);
        updateLocation(i, step);
        step += .6;
    }
    ctx.drawImage(tc, currentX - 50, currentY - 50, 100, 150);
    window.requestAnimationFrame(callback);
}

//draw
window.requestAnimationFrame(callback);


