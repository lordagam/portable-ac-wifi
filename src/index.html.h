constexpr char kIndexContentType[] = "text/html";
constexpr PROGMEM char kIndexHtml[] = R"html(
<!Doctype html>
<html>
<head>
  <title>Portable A/C Thermostat</title>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js"></script>
  <script>
    function updateButtonLabel(btn) {
      if (btn.val() == 'true') {
        btn.text('ON - Click to Disable');
      } else {
        btn.text('OFF - Click to Enable');
      }
    }
    function toggleButton(btn) {
      if (btn.val() == 'true') {
        btn.val('false');
      } else {
        btn.val('true');
      }
      updateButtonLabel(btn);
    }

    function loadSettings(settings) {
      $('#ambient').text(Number(settings.ambient).toFixed(1));
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
      $.post('/settings', settings, (data) => {
        loadSettings(data);
      });
    }

    let settingsInputTimer = undefined;
    function onSettingsInput() {
      $('#ambient').text('Sending..');
      if (settingsInputTimer != undefined) {
        clearTimeout(settingsInputTimer);
      }
      settingsInputTimer = setTimeout(sendSettings, 2000);
    }

    function loadTarget(settings) {
      $('#ambient').text(Number(settings.ambient).toFixed(1));
      $('#targetF').val(settings.thermostatInF);
      updateButtonLabel($('#targetEnabled').val(settings.enabled));
    }
    function sendTarget() {
      let settings = {};
      settings.thermostatInF = $('#targetF').val();
      settings.enabled = $('#targetEnabled').val();
      $.post('/targetCooling', settings, (data) => {
        loadTarget(data);
      });
    }

    let targetInputTimer = undefined;
    function onTargetInput() {
      $('#ambient').text('Sending..');
      if (targetInputTimer != undefined) {
        clearTimeout(targetInputTimer);
      }
      targetInputTimer = setTimeout(sendTarget, 2000);
    }

    $(document).ready(() => {
      $('input.targetInput').on('input', onTargetInput);
      $('button.targetInput').click((evt) => {
        toggleButton($(evt.target));
        onTargetInput();
      });
      $('input.settingsInput').on('input', onSettingsInput);
      $('button.settingsInput').click((evt) => {
        toggleButton($(evt.target));
        onSettingsInput();
      });

      $.getJSON('/settings', (data) => {
        loadSettings(data);
      });
      $.getJSON('/targetCooling', (data) => {
        loadTarget(data);
      });
    });
  </script>
</head>
<body>
  <h1>Portable A/C Thermostat</h1>
  <p><label>Ambient Temperature:</label> <span id="ambient">Loading..</span>&deg;F</p>

  <h2>Target Cooling</h2>
  <p><label>Target Temperature:</label> <input type="number" id="targetF" min="50" max="100" class="targetInput"/>&deg;F</p>
  <p><label>Enabled: </label><button id="targetEnabled" class="targetInput">...</button></p>

  <h2>Manual Settings</h2>
  <p><label>Thermostat Setting:</label> <input type="number" id="thermostat" min="50" max="100" class="settingsInput"/>&deg;F</p>
  <p><label>Mode:</label><br/>
    <input type="radio" name="mode" id="cool" class="settingsInput"/>
    <label for="cool">Cool</label><br/>
    <input type="radio" name="mode" id="dehum" class="settingsInput"/>
    <label for="dehum">Dehumidify</label><br/>
    <input type="radio" name="mode" id="fan_only" class="settingsInput"/>
    <label for="fan_only">Fan Only</label>
  </p>
  <p><label>Fan Speed:</label><br/>
    <input type="radio" name="fan" id="low" class="settingsInput"/>
    <label for="low">Low</label><br/>
    <input type="radio" name="fan" id="medium" class="settingsInput"/>
    <label for="medium">Medium</label><br/>
    <input type="radio" name="fan" id="high" class="settingsInput"/>
    <label for="high">High</label>
  </p>
  <p><label>Timer: </label><button id="timer" class="settingsInput">...</button></p>
  <p><label>Power: </label><button id="power" class="settingsInput">...</button></p>
</body>
</html>
)html";
