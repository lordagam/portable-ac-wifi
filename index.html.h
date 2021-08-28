constexpr char kIndexContentType[] = "text/html";
constexpr PROGMEM char kIndexHtml[] = R"html(
<!Doctype html>
<html>
<head>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js"></script>
  <script>
    function updateButtonLabel(btn) {
      if (btn.val() == 'true') {
        btn.text('Disable');
      } else {
        btn.text('Enable');
      }
    }
    function loadSettings(settings) {
      $('#ambient').text('N/A');
      $('#thermostat').val(settings.thermostatInF);
      $(`#${settings.mode}`).prop('checked', true);
      $(`#${settings.fan}`).prop('checked', true);
      updateButtonLabel($('#timer').val(settings.timer));
      updateButtonLabel($('#power').val(settings.power));
    }
    function sendSettings() {
      let settings = {};
      settings.thermostatInF = $('#thermostat').val();
      settings.mode = $('input[name="mode"]:checked').attr('id');
      settings.fan = $('input[name="fan"]:checked').attr('id');
      settings.timer = $('#timer').val();
      settings.power = $('#power').val();
      // FIXME
      console.log(settings);
      $.post('/settings', settings, (data) => {
        loadSettings(data);
      });
    }

    let inputTimer = undefined;
    function onInput() {
      // FIXME
      console.log('onInput');
      $('#ambient').text('Sending..');
      if (inputTimer != undefined) {
        clearTimeout(inputTimer);
      }
      inputTimer = setTimeout(sendSettings, 2000);
    }

    $(document).ready(() => {
      $('input').on('input', onInput);
      $('button').click((evt) => {
        const btn = $(evt.target);
        if (btn.val() == 'true') {
          btn.val('false');
        } else {
          btn.val('true');
        }
        updateButtonLabel(btn);
        onInput();
      });

      $.getJSON('/settings', (data) => {
        loadSettings(data);
      });
    });
  </script>
</head>
<body>
  <h1>Portable A/C Thermostat</h1>
  <p><label>Ambient Temperature:</label> <span id="ambient">Loading..</span></p>
  <p><label>Thermostat Setting:</label> <input type="number" id="thermostat" min="50" max="100"/></p>
  <p><label>Mode:</label>
    <input type="radio" name="mode" id="cool"/>
    <label for="cool">Cool</label>
    <input type="radio" name="mode" id="dehum"/>
    <label for="dehum">Dehumidify</label>
    <input type="radio" name="mode" id="fan_only"/>
    <label for="fan_only">Fan Only</label>
  </p>
  <p><label>Fan Speed:</label>
    <input type="radio" name="fan" id="low"/>
    <label for="low">Low</label>
    <input type="radio" name="fan" id="medium"/>
    <label for="medium">Medium</label>
    <input type="radio" name="fan" id="high"/>
    <label for="high">High</label>
  </p>
  <p><label>Timer: </label><button id="timer">...</button></p>
  <p><label>Power: </label><button id="power">...</button></p>
</body>
</html>
)html";
