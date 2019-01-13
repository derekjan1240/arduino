const request = require('request');
const cheerio = require('cheerio');
const com = require("serialport");
const express = require('express');
const events = require('events');
var socket = require('socket.io');



//create a server ----------------------------------------------------
var app = express();
var path = require("path");

app.get('/', function (req, res) {
    res.sendFile(path.join(__dirname + '/public/home.html'));
    });

var server = app.listen(4000, function(){
    console.log('listening for requests on port 4000!');
    });

var io = socket(server);
app.use(express.static('public'));

//create a EventEmitter ---------------------
var eventEmitter = new events.EventEmitter();

//serialPort communication (arduino & pc) -----------------------------
const serialPort = new com("COM4", {
  baudRate: 9600
});

var url, keyword;

   //wait logging event
    eventEmitter.on('logging', function(message) {
      io.emit('log_message', message);
    });

    // Override console.log
    var originConsoleLog = console.log;
    console.log = function(data) {
      eventEmitter.emit('logging', data);
      originConsoleLog(data);
    };


  serialPort.on("open", function(){
    console.log("已開啟序列埠");
  
    serialPort.on("data", function(d){

      //console.log("資料：" + d);             // 顯示傳入序列埠的資料
      //console.log(typeof(d));                //object
      d = d.toString();
               

      if(d == 1) //pm2.5
      {
          $ = cheerio.load('<h1 id = "content"> data </h1>');
          $('h1.content').text('Hello there!')

          url = 'https://taqm.epa.gov.tw/pm25/tw/PM25A.aspx?area=4';
          keyword = '.TABLE_G tbody tr'; 
      }
      
      else if(d == 2) //氣象
      {
          url = 'http://www.cwb.gov.tw/V7/forecast/taiwan/Taichung_City.htm';
          keyword = '#box8 .FcstBoxTable01 tbody tr';
      }
      else if(d == 3) //ptt_ntuttalk
      {
          url = 'https://www.ptt.cc/bbs/NTUT_Talk/index.html';
          keyword = '#main-container .r-ent .title';
      }

      
      if(d == 3||d == 1||d == 2)
      {  
        request(url, (err, res, body) => {
  
          const $ = cheerio.load(body);
          var result = [];

          $(keyword).each(function(i, elem) {
            result.push($(this).text().split('\n'))
          })      
    
          //排版
          if(d == 1) //pm2.5
          {
              pm25 = result.map(result => ({
              place: result[2].replace(/\s+/g, ""),
              current: result[4].replace(/\s+/g, ""),
              last_hour: result[6].replace(/\s+/g, ""),
              }))

              //for(j = 1 ;j<12; j++){
                //console.log(pm25[j]);
              //}
          
              console.log(
                "----- pm2.5 ----" + "<br>" + "<br>" + 
                pm25[1].place.toString() + " " + pm25[1].current.toString()+ " " + pm25[1].last_hour.toString() + "<br>"+ 
                pm25[2].place.toString() + " " + pm25[2].current.toString()+ " " + pm25[2].last_hour.toString() + "<br>"+
                pm25[3].place.toString() + " " + pm25[3].current.toString()+ " " + pm25[3].last_hour.toString() + "<br>"+
                pm25[4].place.toString() + " " + pm25[4].current.toString()+ " " + pm25[4].last_hour.toString() + "<br>"+
                pm25[5].place.toString() + " " + pm25[5].current.toString()+ " " + pm25[5].last_hour.toString() + "<br>"+
                pm25[6].place.toString() + " " + pm25[6].current.toString()+ " " + pm25[6].last_hour.toString() + "<br>"+
                pm25[7].place.toString() + " " + pm25[7].current.toString()+ " " + pm25[7].last_hour.toString() + "<br>"+
                pm25[8].place.toString() + " " + pm25[8].current.toString()+ " " + pm25[8].last_hour.toString() + "<br>"+
                pm25[9].place.toString() + " " + pm25[9].current.toString()+ " " + pm25[9].last_hour.toString() + "<br>"+
                pm25[10].place.toString() + " " + pm25[10].current.toString()+ " " + pm25[10].last_hour.toString() +"<br>"+
                pm25[11].place.toString() + " " + pm25[11].current.toString()+ " " + pm25[11].last_hour.toString()
              )
              
          }
     
          else if(d == 2) //wheather
          {         
              weathers = result.map(result => ({
              time: result[1].substring(2).split(' ')[0],
              temperature: result[2].substring(2),
              feel: result[5].substring(2),
              rain: result[6].substring(2),  
              }))

              //console.log(weathers);

              console.log(
                "---- weathers ----" + "<br>" + "<br>" + 
                weathers[0].time.toString() + " " + weathers[0].feel.toString()+ " " + weathers[0].rain.toString() + "<br>"+ 
                weathers[1].time.toString() + " " + weathers[1].feel.toString()+ " " + weathers[1].rain.toString() + "<br>"+
                weathers[2].time.toString() + " " + weathers[2].feel.toString()+ " " + weathers[2].rain.toString()             
              )

          
          }
      
          else if(d == 3) //pm2.5
          {
              //ptt
              ptt_ntuttalk = result.map(result => ({
              NTUTTalk: result[2].substring(4),
              }))

              //console.log(ptt_ntuttalk);
              console.log(
                "---- ptt_ntuttalk ----" + "<br>" + "<br>" + 
                ptt_ntuttalk[0].NTUTTalk.toString() + "<br>"+ 
                ptt_ntuttalk[1].NTUTTalk.toString() + "<br>"+
                ptt_ntuttalk[2].NTUTTalk.toString() + "<br>"+ 
                ptt_ntuttalk[3].NTUTTalk.toString() + "<br>"+
                ptt_ntuttalk[4].NTUTTalk.toString() + "<br>"+ 
                ptt_ntuttalk[5].NTUTTalk.toString() + "<br>"+
                ptt_ntuttalk[6].NTUTTalk.toString() + "<br>"+ 
                ptt_ntuttalk[7].NTUTTalk.toString() + "<br>"+
                ptt_ntuttalk[8].NTUTTalk.toString() + "<br>"+ 
                ptt_ntuttalk[9].NTUTTalk.toString() + "<br>"+
                ptt_ntuttalk[10].NTUTTalk.toString()+ "<br>"+ 
                ptt_ntuttalk[11].NTUTTalk.toString()+ "<br>"+
                ptt_ntuttalk[12].NTUTTalk.toString()+ "<br>"+
                ptt_ntuttalk[13].NTUTTalk.toString()              
              )
          }

        });
      }
    })
  });  

console.log("等待開啟序列埠 ~");

      

/* --------------------------------------------------
var com = require("serialport");
// 請自行修改序列埠設定
var serialPort = new com("COM3", {
    baudRate: 9600
  });

serialPort.on("open", function(){
  console.log("已開啟序列埠");
  
  serialPort.on("data", function(d){
      console.log("資料：" + d);             // 顯示傳入序列埠的資料
  });
});
--------------------------------------------------  */