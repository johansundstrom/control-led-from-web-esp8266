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
  </div>
  
  <script>
     async function postData(){

      let myData = {}; 
      const button = document.querySelector("#button");
    
      //   datalast
      if (button.classList.contains('css_off')) {
        myData = { state: 'ON' };
      } else {
        myData = { state: 'OFF' };
      }
        
      let epoch = new Date().getTime();
      myData.clientEpoch = epoch;
      //   /datalast
      
      const options = {
        method: 'POST',
        mode: 'cors',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(myData)
      }
    
      const response = await fetch('/led', options);
      const jsonData = await response.json();
      console.log(jsonData);
    
      if (button.classList.contains('css_off')) {
        button.classList.remove('css_off');
        button.classList.add('css_on');
        button.innerHTML = 'LED ON';
      } else {
        button.classList.remove('css_on');
        button.classList.add('css_off');
        button.innerHTML = 'LED OFF';
      }
    }
  </script>
</body>

</html>
)====";
