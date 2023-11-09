#include "func.h"
////////////////////////////////////////////////////////
IRsend irsend(kIrLed);
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
FastBot bot;
Data dat;
FileData data(&LittleFS, "/data.dat", 'B', &dat, sizeof(dat));
ESP8266WebServer server(80);
////////////////////////////////////////////////////////
int button;
bool sta = 0;
////////////////////////////////////////////////////////
void editbt() {//Запись в глобальную переменную (кринж) id редактируемой кнопки
  if (server.args() > 0) {
    button = server.arg("but").toInt();
  }
  server.send(200, "text/html", editpage);
}

void handleRoot() {//Основная страница
  if (sta) {
    server.send(200, "text/html", wifi);
  } else {
    server.send(200, "text/html", webpage);
  }
}

void readIR() {//Чтение ИК сигнала
  decode_results results;
  while (1) {
    yield();
    if (irrecv.decode(&results)) {
      yield();
      uint32_t now = millis();
      String description = IRAcUtils::resultAcToString(&results);
      if (description.length()) Serial.println("Description: " + description);
      yield();//больше yield()
      write_to_arr(resultToRawArray(&results), getCorrectedRawLength(&results), button);
      server.send(200, "text/plane", "Успех" + (String)getCorrectedRawLength(&results));
      yield();//еще больше yield()
      break;
    }
  }
  server.send(200, "text/plane", "Успех");
}

void del() {//удаление кнопки
  String id = server.arg("id");
  if (id == "100") id = "0";
  for (int i = id.toInt(); i < NUM_BUTTONS - 1; i++) {
    write_to_str(dat.names[i + 1], i);
    write_to_arr(dat.codes[i + 1], 199, i);
  }
  dat.names[NUM_BUTTONS - 1][0] = '\0';
  data.update();
}

void save() {//Запись имени кнопки
  String name = server.arg("name");
  write_to_str(name, button);
}

void nameread() {//отправка всех имен кнопок
  String resp;
  for (int i = 0; i < NUM_BUTTONS; i++) {
    resp = resp + dat.names[i] + "&&&";
  }
  server.send(200, "text/plane", resp);
}

void sendIR() {//отправка ИК сигнала
  String ir = server.arg("ir"); //Чтение значение заголовка "ir"
  irsend.sendRaw(dat.codes[ir.toInt()], 199, 38); //Отправка ИК сигнала
}

void getName() {//отправка имени редактируемой кнопки
  server.send(200, "text/html", dat.names[button]); //Отправка данных на веб-страницу
}

void gettg() {//отправка тг конфига
  server.send(200, "text/html", (String)dat.tg + String("&&&") + dat.token);
}
void tgdata() {//получение тг кнфига
  bool tg = (bool)server.arg("tg").toInt();
  String token = server.arg("token");
  token.toCharArray(dat.token, 47);
  dat.tg = tg;
  data.update();
  bot.setToken(token);
}

void wifih(){//получение данных о вайфай сети
  server.arg("ssid").toCharArray(dat.ssid, 40);
  server.arg("pass").toCharArray(dat.pass, 40);
  Serial.println(dat.ssid);
  Serial.println(dat.pass);
  data.update();
  setupWiFi();
}


void newMsg(FB_msg& msg) {//обработка входящих сообщений тг
  String chatID = msg.chatID;
  String text = msg.text;
  String menu;
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if ((i + 1) % 4 == 0) {
      menu = menu + dat.names[i] + "\n";
    } else {
      menu = menu + dat.names[i] + "\t";
    }
  }
  if (text == "/start") {
    bot.sendMessage("Hello!", chatID);
    bot.showMenu(menu, chatID);
  } else {
    for (int i = 0; i < NUM_BUTTONS; i++) {
      if (text == dat.names[i]) {
        irsend.sendRaw(dat.codes[i], 199, 38);
         bot.sendMessage("Команда " + String(dat.names[i]) + " отправлена", chatID);
         bot.showMenu(menu, chatID);
      }
    }
  }
}

void write_to_str(String text, int str) {//запись имени кнопки в структуру
  text.toCharArray(dat.names[str], 40);
  data.update();
}

void write_to_arr(uint16_t* ptr, int len, int str) {//запись ИК кода кнопки в структуру
  for (int i = 0; i < len; i++) {
    dat.codes[str][i] = ptr[i];
    yield();
  }
  data.update();
}

void setupWiFi(){//Подключение вайфай 
  WiFi.begin(dat.ssid, dat.pass);
  for (int i = 0; i < 150; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(dat.ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      sta = 0;
      break;
    }
    Serial.print(".");
    delay(100);
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.softAP("ESP8266", "12345678");
    sta = 1;
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  }
}

void config() {//инициализация
  irsend.begin();  // инициализируем ИК передатчик
  Serial.begin(115200);

  server.on("/", handleRoot);
  server.on("/edit", editbt);
  server.on("/readIR", readIR);
  server.on("/save", save);
  server.on("/del", del);
  server.on("/nameread", nameread);
  server.on("/sendIR", sendIR);
  server.on("/getName", getName);
  server.on("/tgdata", tgdata);
  server.on("/gettg", gettg);
  server.on("/wifi", wifih);

  server.begin();

  bot.attach(newMsg);

  LittleFS.begin();
  FDstat_t stat = data.read();

#if DECODE_HASH
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif
  irrecv.enableIRIn();
  bot.setToken(dat.token);
}

void tick() { //тикер
  if (dat.tg) {
    bot.tick();
  }
  data.tick();
  server.handleClient();
}