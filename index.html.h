constexpr char kIndexContentType[] = "text/html";
constexpr PROGMEM char kIndexHtml[] = R"html(
<!Doctype html>
<html>
<head>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js"></script>
  <script>
    // TODO: populate initial state
    // TODO: add change handlers
  </script>
</head>
<body>
  <h1>Portable A/C Thermostat</h1>
  <p><label>Ambient Temperature:</label> <span id="ambient">Loading..</span></p>
  <p><label>Thermostat Setting:</label> <input type="number" id="thermostat"/></p>
  <p><label>Mode:</label>
    <input type="radio" name="mode" id="cool"/>
    <label for="cool">Cool</label>
    <input type="radio" name="mode" id="dehum"/>
    <label for="dehum">Dehumidify</label>
    <input type="radio" name="mode" id="fan_only"/>
    <label for="fanonly">Fan Only</label>
  </p>
  <p><label>Fan Speed:</label>
    <input type="radio" name="fan" id="low"/>
    <label for="low">Low</label>
    <input type="radio" name="fan" id="medium"/>
    <label for="medium">Medium</label>
    <input type="radio" name="fan" id="high"/>
    <label for="high">High</label>
  </p>
  <p><button id="timer"/></p>
  <p><button id="power"/></p>
</body>
</html>
)html";
