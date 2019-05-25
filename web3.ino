#include <SPI.h>//引用SPI头文件
#include <ESP8266WiFi.h>
#include <dht11.h>

const char *ssid = "SSSS";
const char *password = "sun123hao";

WiFiServer server(80);
WiFiClient client;
String readString = "";
#define DHT11PIN 14
int Light=4;
int Sensor=A0;
dht11 DHT11;
//响应头
String responseHeaders =
    String("") +
    "HTTP/1.1 200 OK\r\n" +
    "Content-Type: text/html\r\n" +
    "Connection: close\r\n" +
    "\r\n";

//网页
String myhtmlPage =
    String("") +
    "<html>"+
    "<head>"+
    "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"+
    "<title>智能温湿度照明度监测系统控制台</title>"+
    "<script defer=\"defer\">" +
    "function ledSwitchOn() {"+
    "var xmlhttp;"+
    "if (window.XMLHttpRequest) {"+
    "xmlhttp = new XMLHttpRequest();"+
    "} else {"+
    "xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");" +
    "}"+
    "xmlhttp.onreadystatechange = function () {"+
    "if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {"+
    "document.getElementById(\"txtState\").innerHTML = xmlhttp.responseText;" +
    "}"+
    "},"+
    "xmlhttp.open(\"GET\", \"开灯\", true);" +
    "xmlhttp.send(); "+
    "}"+
    "function ledSwitchOff() {"+
    "var xmlhttp;"+
    "if (window.XMLHttpRequest) {"+
    "xmlhttp = new XMLHttpRequest();"+
    "} else {"+
    "xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");" +
    "}"+
    "xmlhttp.onreadystatechange = function () {"+
    "if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {"+
    "document.getElementById(\"txtState\").innerHTML = xmlhttp.responseText;" +
    "}"+
    "},"+
    "xmlhttp.open(\"GET\", \"关灯\", true);" +
    "xmlhttp.send(); "+
    "}"+
    "function getBrightness() {"+
    "var xmlhttp;"+
    "if (window.XMLHttpRequest) {"+
    "xmlhttp = new XMLHttpRequest();"+
    "} else {"+
    "xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");" +
    "}"+
    "xmlhttp.onreadystatechange = function () {"+
    "if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {"+
    "document.getElementById(\"guangzhao\").innerHTML = xmlhttp.responseText;" +
    "}"+
    "},"+
    "xmlhttp.open(\"GET\", \"?getBrightness\", true);" +
    "xmlhttp.send(); "+
    "}"+
    "window.setInterval(getBrightness,1000);"+
    "function wendu() {"+
    "var xmlhttp;"+
    "if (window.XMLHttpRequest) {"+
    "xmlhttp = new XMLHttpRequest();"+
    "} else {"+
    "xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");" +
    "}"+
    "xmlhttp.onreadystatechange = function () {"+
    "if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {"+
    "document.getElementById(\"wendu\").innerHTML = xmlhttp.responseText;" +
    "}"+
    "},"+
    "xmlhttp.open(\"GET\", \"?wendu\", true);" +
    "xmlhttp.send(); "+
    "}"+
    "window.setInterval(wendu,1000);"+
    "function shidu() {"+
    "var xmlhttp;"+
    "if (window.XMLHttpRequest) {"+
    "xmlhttp = new XMLHttpRequest();"+
    "} else {"+
    "xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");" +
    "}"+
    "xmlhttp.onreadystatechange = function () {"+
    "if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {"+
    "document.getElementById(\"shidu\").innerHTML = xmlhttp.responseText;" +
    "}"+
    "},"+
    "xmlhttp.open(\"GET\", \"?shidu\", true);" +
    "xmlhttp.send(); "+
    "}"+
    "window.setInterval(shidu,1000);"+
    "</script>"+
    "</head>"+
    "<body>"+
    "<div style=\"width:1160px; height:270px; text-align:center;line-height:20px;border:white solid 1px;font-size:90px;\">"+
    "<p>智能温室大棚环境监测系统</p><p>控制台</p>"+
    "</div>"+
    "<div class>"+
    "<div style=\"float:left;width:550px; height:250px; text-align:center; line-height:250px; border:black solid 1px;font-size:90px;margin-left:10px;margin-top:30px;\">"+
    "<div id=\"wendu\">"+
    "温度："+
    "</div><div style=\"float:left;width:550px; height:250px; text-align:center; line-height:250px; border:black solid 1px;font-size:90px;margin-left:590px;margin-top: -250px;\">"+
    "<div id=\"shidu\">"+
    "湿度："+
    "</div></div>"+
    "<div class>"+
    "<div style=\"float:left;width:550px; height:250px; text-align:center; line-height:250px; border:black solid 1px;font-size:90px;margin-left:0px;margin-top:30px;\">"+
    "<div id=\"guangzhao\">"+
    "照度："+
    "</div><div style=\"float:left;width:550px; height:250px; text-align:center; line-height:250px; border:black solid 1px;font-size:90px;margin-left:590px;margin-top: -250px;\">"+
    "<div id=\"txtState\">"+
    "设备在线"+
    "</div></div>"+
    "<div>"+
    "<input type=\"button\" value=\"开 灯\" onclick=\"ledSwitchOn()\"style=\"height:200px;width:400px;display:inline-block;font-size:70px;cursor:pointer;margin-top: 30px;\">"+
    "<input type=\"button\" value=\"关 灯\" onclick=\"ledSwitchOff()\"style=\"height:200px;width:400px;display:inline-block;font-size:70px;cursor:pointer;margin-top: 30px;\">"+
    "</div>"+
    "<div id=\"dqtime\">"+
    "<div style=\"float:left;width:550px; height:55px; text-align:center; line-height:1px;font-size:45px;margin-left:590px;margin-top: -260px;\">"+
    "当前时间"+
    "</div>"+
    "<div id=\"time\">"+
    "<div style=\"float:left;width:550px; height:200px; text-align:center; line-height:100px;font-size:45px;margin-left:590px;margin-top: -240px;\">"+
    "<span id=\"cg\">-------------------------</span>"+
"<script>"+
"setInterval(\"cg.innerHTML=new Date().toLocaleString()\",1000);"+
"</script>"+
"</div>"+
"<div id=\"about\">"+
"<div style=\"float:left;margin-left:980px;margin-top: -70px;\">"+
"<button type=\"button\" onclick=\"alert('作者：孙浩（沈阳城市建设学院）     版本：V2.0')\" style=\"height:80px;width:160px;display:inline-block;cursor:pointer;font-size:30px;\">关 于</button>"+
"</div>"+
    "</body>"+
    "</html>";

void setup()
{
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH); // 熄灭LED
    Serial.begin(115200);
    Serial.println();

    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected");

    server.begin();
    Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
}

void loop()
{
  int chk = DHT11.read(DHT11PIN);
    WiFiClient client = server.available(); //尝试建立客户对象
    if (client)                             //如果当前有客户可用
    {
        boolean currentLineIsBlank = true;
        Serial.println("[Client connected]");

        while (client.connected()) //如果客户端建立连接
        {
            if (client.available()) //等待有可读数据
            {
                char c = client.read(); //读取一字节数据
                readString += c;        //拼接数据
                if (c == '\n' && currentLineIsBlank) //等待请求头接收完成(接收到空行)
                {
                    //比较接收到的请求数据
                    if (readString.startsWith("GET / HTTP/1.1")) //如果是网页请求
                    {
                        client.print(responseHeaders); //向客户端输出网页响应
                        client.print(myhtmlPage);      //向客户端输出网页内容
                        client.print("\r\n");
                    }
                    else
                    {
                        if (readString.startsWith("GET /%E5%BC%80%E7%81%AF"))
                        {
                            digitalWrite(4, LOW); // 点亮LED
                            client.print("灯已打开");
                        }
                        if (readString.startsWith("GET /%E5%85%B3%E7%81%AF"))
                        {
                            digitalWrite(4, HIGH); // 熄灭LED
                            client.print("灯已关闭");
                        }
                        if (readString.startsWith("GET /?getBrightness"))
                        {
                          client.print("照度:");
                          client.print(analogRead(Sensor));
                            client.print("lx");
                        }
                        if (readString.startsWith("GET /?wendu"))
                        {
                          client.print("温度:");
                            client.print(DHT11.temperature);
                            client.print("℃");
                        }
                        if (readString.startsWith("GET /?shidu"))
                        {
                          client.print("湿度:");
                            client.print(DHT11.humidity);
                            client.print("%");
                        }
                    }
                    break;
                }

                if (c == '\n')
                {
                    currentLineIsBlank = true; //开始新行
                }
                else if (c != '\r')
                {
                    currentLineIsBlank = false; //正在接收某行中
                }
            }
        }
        delay(1);      //等待客户完成接收
        //client.stop(); //结束当前连接:
        Serial.println("[Client disconnected]");
        Serial.println(readString); //打印输出来自客户的数据
        readString = "";
    }
}
