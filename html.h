const char editpage[] PROGMEM = R"=====(
<!doctype html>
<html lang="ru">

<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Редактирование кнопки</title>
</head>
<style type="text/css">
    body {
        background-color: #292929;
        color: aliceblue;
    }

    #name {
        border: none;
        padding: 1%;
        background-color: rgb(16, 16, 16);
        color: aliceblue;
        margin-top: 1%;
        min-height: 50px;
        min-width: 200px;
        border-radius: 10px;
    }

    .button {
        width: auto;
        background-color: #39823b;
        color: aliceblue;
        padding: 1%;
        border: none;
        border-radius: 10px;
        transition: all 0.3s;
        min-height: 40px;
        min-width: 100px;
    }

    .button:hover {
        background: #ffffff;
        color: black;
    }

    .button:active {
        background-color: #39823b;
        color: white;
    }

    #code {
        margin-top: 2%;
        margin-bottom: 2%;
    }
</style>

<body>
    <center>
        <h1>
            Редактирование кнопки
        </h1>
        <form>
            <label id="label"> Название кнопки
                <br>
                <input type="text" id="name" placeholder="20 символов максимум">
            </label>
        </form>
        <div id="code">&nbsp</div>
        <button class="button" onclick=readIR()>
            Считать код с приемника
        </button>
        <br>
        <br>
        <a href="/">
            <button class="button" onclick=save()>
                Сохранить
            </button>
        </a>
    </center>

    <script>
        function readIR() {
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "readIR", true);
            xhttp.send();
            document.getElementById("code").innerHTML = "Ожидание ИК сигнала..."
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    var data = this.responseText;
                    document.getElementById("code").innerHTML = data;
                }
            }
        }

        function save() {
            var xhttp = new XMLHttpRequest();
            xhttp.open("POST", "save?name=" + (document.getElementById("name").value), true);
            xhttp.send();
        }
        function getName() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    var data = this.responseText;
                    document.getElementById("name").value = data;
                }
            }
            xhttp.open("GET", "getName", true);
            xhttp.send();
        }
        getName();
    </script>
</body>

</html>)=====";


const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang='ru-RU'>

<head>
    <meta charset='UTF-8' />
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Универсальный ИК пульт</title>
</head>

<style type="text/css">
    body {
        color: aliceblue;
        background-color: #292929;
    }

    .wrapper {
        display: grid;
        grid-template-columns: repeat(auto-fit, minmax(305px, 1fr));
        align-content: space-evenly;
        justify-content: center;
        align-items: center;
        justify-items: stretch;
    }

    .but {
        background-color: #4CAF50;
        min-height: 50px;
        border-radius: 10px 10px 0px 0px;
        word-break: break-all;
        transition: all 0.3s;
    }

    .but:hover {
        background: #ffffff;
        color: black;
    }

    .but h5 {
        text-align: start;
        margin: 5%;
        padding: 2%;
    }

    .button {
        float: left;
        width: 50%;
        background-color: #39823b;
        color: aliceblue;
        padding: 2%;
        border: none;
        border-radius: 0px 0px 0px 10px;
        transition: all 0.3s;
    }

    .button:hover {
        background: #ffffff;
        color: black;
    }

    .button:active {
        background-color: #39823b;
        color: aliceblue;
    }

    .but:active {
        background-color: #39823b;
        color: aliceblue;
    }

    .obr {
        padding: 5%;
    }

    .add {
        height: 100%;
        background-color: #39823b00;
        border-color: #39823b;
        color: aliceblue;
        font-size: 8vh;
        border-radius: 10px;
        border-width: 2px;
        border-style: dashed;
    }

    #del {
        border-radius: 0px 0px 10px 0px;
    }

    .dropdown {
        float: left;
    }

    .dropbtn {
        background: none;
        border: none;
        color: aliceblue;
        text-decoration: underline;
        border-left-style: solid;
    }

    .dropdown-content {
        display: none;
        position: absolute;
        border-left-style: solid;
    }

    .dropdown-content label {
        color: aliceblue;
        padding: 12px 16px;
        text-decoration: none;
        display: block;
        text-align: left;
        text-wrap: nowrap;
    }

    .dropbtn span {
        display: inline-block;
        transition: all 0.3s;
    }

    #token {
        background-color: #292929;
        border-radius: 5px;
        border: 1px solid aliceblue;
        color: aliceblue;
        padding: 2%;
    }

    #token:focus-visible {
        outline: none;
        border: 1px solid #39823b;
    }

    .show {
        display: block;
    }

    .rotate {
        transform: rotate(90deg);
    }
    .custom-checkbox {
        display: none;
    }

    .custom-checkbox+label {
        display: flex;
        align-items: center;
    }

    .custom-checkbox+label::before {
        content: '';
        display: inline-block;
        width: 15px;
        height: 15px;
        flex-shrink: 0;
        flex-grow: 0;
        border: 1px solid aliceblue;
        border-radius: 0.25em;
        margin-right: 0.5em;
        background-repeat: no-repeat;
        background-position: center center;
        background-size: 50% 50%;
    }

    .custom-checkbox+label::after {
        order: -1;
        content: '';
        width: 12px;
        height: 12px;
        border: none;
        border-radius: 0.25em;
        background: blue;
        position: absolute;
        left: 18px;
        top: 15px;
        display: none;

    }

    .custom-checkbox:checked+label::after {
        display: block;
        background: #39823b;
    }
</style>


<body>
    <center>
        <h1>Универсальный ИК пульт</h1>
        <div class="wrapper">
        </div>

        <div class="dropdown">
            <button class="dropbtn" onclick="show()">
                <span id="triangle">&#9658;</span> Конфиг
            </button>

            <div id="myDropdown" class="dropdown-content">
                <form onsubmit="subm(form.ch.checked, form.token.value)" , id="form">
                    <div class="check">
                        <input type="checkbox" class="custom-checkbox" id="ch" name="ch" value="yes">
                        <label for="ch">Тг бот</label>
                    </div>

                    <label>
                        Токен бота:
                        <input type="text" id="token" placeholder="Токен бота">
                    </label>
                    <button>
                        Сохранить
                    </button>
                </form>
            </div>
        </div>
    </center>
</body>


<script>
    function show() {
        document.getElementById("myDropdown").classList.toggle("show");
        document.getElementById("triangle").classList.toggle("rotate");
    }
    function send(but) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("POST", "edit?but=" + but, true);
        xhttp.send();
    }

    function getData() {
        document.getElementsByClassName("wrapper")[0].innerHTML =
            `<div class="obr" id="add">
                <div onclick="add()" class="add">
                     +
                </div>
            </div>`;
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                var data = this.responseText;
                var dataS = data.split("&&&");
                for (let i = 0; i < dataS.length - 1; i++) {
                    if (dataS[i] != "") {
                        create(i, dataS[i]);
                    }
                }

            }
        };
        xhttp.open("GET", "nameread", true);
        xhttp.send();
    }

    function create(id, name) {
        const container = document.querySelector('#add');
        const obr = document.createElement('div');
        obr.classList.add('obr');
        obr.innerHTML =
            `<div class="but" onclick=sendIR(${id})>
                <h5 id="${id}"> ${name}</h5>
            </div>
            <a href="edit">
                <button class="button" onclick="send(${id})">Редактировать</button>
            </a>
            <button class="button" id="del" onclick="del(${id})">Удалить</button>`;
        container.before(obr);
    }

    function sendIR(ir) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("POST", "sendIR?ir=" + ir, true);
        xhttp.send();
    }

    function add() {
        var count = document.getElementsByClassName("obr").length;
        create(count - 1, "empty");
    }

    function del(id) {
        var xhttp = new XMLHttpRequest();
        xhttp.open("POST", "del?id=" + id, true);
        xhttp.send();
        getData();
    }

    let form = document.getElementById("form");

    function subm(check, token) {
        event.preventDefault();
        var xhttp = new XMLHttpRequest();
        xhttp.open("POST", "tgdata?tg=" + Number(check) + "&token=" + token, true);
        xhttp.send();
    }

    function gettg() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                let form = document.getElementById("form");
                var dataS = this.responseText.split("&&&");
                form.ch.checked = Boolean(Number(dataS[0]));
                form.token.value = dataS[1];
            }
        };
        xhttp.open("GET", "gettg", true);
        xhttp.send();
    }

    getData();
    gettg();
</script>

</html>
)=====";



const char wifi[] PROGMEM = R"=====(
<!doctype html>
<html lang="ru">

<head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Вход</title>
</head>
<style type="text/css">
    body {
        background-color: #292929;
        color: aliceblue;
    }

    input {
        border: none;
        padding: 1%;
        background-color: rgb(16, 16, 16);
        color: aliceblue;
        margin-top: 1%;
        min-height: 30px;
        min-width: 200px;
        border-radius: 10px;
    }

    .button {
        width: auto;
        background-color: #39823b;
        color: aliceblue;
        padding: 1%;
        border: none;
        border-radius: 10px;
        transition: all 0.3s;
        min-height: 40px;
        min-width: 100px;
    }

    .button:hover {
        background: #ffffff;
        color: black;
    }

    .button:active {
        background-color: #39823b;
        color: white;
    }
</style>

<body>
    <center>
        <h1>
            Введите данные Wi-Fi сети
        </h1>
        <form>
            <label id="ssidlabel"> Ssid
                <br>
                <input type="text" id="ssid" placeholder="ssid">
            </label>
            <br>
            <label id="passlabel"> password
                <br>
                <input type="text" id="pass" placeholder="pass">
            </label>
        </form>
        <br>
        <button class="button" onclick=save()>
            Сохранить
        </button>
    </center>

    <script>
        function save() {
            var ssid = (document.getElementById("ssid"))
            var pass = (document.getElementById("pass"))
            var xhttp = new XMLHttpRequest();
            xhttp.open("POST", "wifi?ssid=" + (document.getElementById("ssid").value) + "\0&pass=" + (document.getElementById("pass").value) + "\0", true);
            xhttp.send();
            document.getElementsByTagName("center")[0].innerHTML = `
            <h1>
              Данные отправлены
            </h1>
            <p>
              Подключитесь к вашей Wi-Fi сети и перезагрузите страницу    
            </p>
            `;
        }
    </script>
</body>

</html>)=====";