const char MAIN_page[] PROGMEM = R"====(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Web controlled LED</title>
  <style>
    #button {
      cursor: pointer;
      padding: 20px 40px;
      font-size: 20px;
      border-radius: 32px;
    }
    
    #center {
      text-align: center;
    }
    
    .css_on {
      background-color: green;
      color: white;
    }
    
    .css_off {
      background-color: red;
      color: black;
    }
  </style>
</head>

<body>
  <div class="center">
    <!--input type='button' id="button" class="css_off" value="LED Off"-->
    <button onclick="postData()" id="button" class="css_off">LED off</button>
    <div id="info"></div>
  </div>
  
  <script>
    let myData = { state: 'OFF' };
    let host;
  
    async function postData() {
    
      const button = document.querySelector("#button");
    
      if (location.host == 'cryptic-meadow-42908.herokuapp.com') {
        host = 'host in United States'
      } else {
        host = 'Localhost';
      }
    
      button.classList.remove('css_off', 'css_on');
      button.classList.add('css_wait');
      button.innerHTML = 'WORKING';
    
      //   datalast
      if (myData.state == 'OFF') {
        myData = { state: 'ON' };
      } else {
        myData = { state: 'OFF' };
      }
    
      let epoch = new Date().getTime();
      myData.clientEpochInMilliSec = epoch;
      //   /datalast
    
      //https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API/Using_Fetch
      const options = {
        method: 'POST',
        mode: 'cors',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(myData)
      }
    
      const response = await fetch('/api', options);
      const jsonData = await response.json();
    
      //template literals - https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals
      document.getElementById('info').innerText = `Clicked element updated after ${jsonData.roundTripInMilliSec}mS roundtrip from ${host}`;
    
      console.log(jsonData);
    
      if (jsonData.state == 'ON') {
        button.classList.remove('css_wait');
        button.classList.add('css_on');
        button.innerHTML = 'LED ON';
      } else {
        button.classList.remove('css_wait');
        button.classList.add('css_off');
        button.innerHTML = 'LED OFF';
      }
    }
  </script>
</body>

</html>
)====";
