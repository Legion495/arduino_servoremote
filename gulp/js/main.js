$(document).ready(function() {
  motor = false;
  gyro.frequency = 250;
  gyro.startTracking(function(o) { tracking(o); });

  function clamp(number, min, max) {
    return Math.max(min, Math.min(number, max));
  }

  function per2ppm(perc) {
    if(perc < 0 && perc >= -1) {
      return Math.round(1600 - perc * 1600 * -1);
    } else if(perc > 0 && perc <= 1) {
      return Math.round(1600 + perc * 1600);
    } else {
      return 1600;
    }
  }

  function tracking(o) {
    steer_l = clamp(o.x *  0.25, -1, 1);
    steer_r = clamp(o.x * -0.25, -1, 1);
    speed_l = clamp(o.y * -0.25, -1, 1);
    speed_r = clamp(o.y * -0.25, -1, 1);
    result_l = 0;
    result_r = 0;

    if(steer_l > 0.1 ) {
      if(speed_l >= 0) {
        result_l = ((speed_l + steer_r) / 2);
        result_r = ((speed_r - steer_r) / 2);
      } else {
        result_l = ((speed_l - steer_r) / 2);
        result_r = ((speed_r + steer_r) / 2);
      }
    } else if(steer_r > 0.1) {
      if(speed_r >= 0) {
        result_l = ((speed_l + steer_r) / 2);
        result_r = ((speed_r - steer_r) / 2);
      } else {
        result_l = ((speed_l - steer_r) / 2);
        result_r = ((speed_r + steer_r) / 2);
      }
    }  else {
      result_l = speed_l;
      result_r = speed_r;
    }

    if(motor) {
      $.ajax({url: "motor?l=" + result_l.toFixed(2) + "&r=" + result_r.toFixed(2)});
    }

    $("#steerl").text(steer_l.toFixed(2));
    $("#steerr").text(steer_r.toFixed(2));
    $("#speedl").text(speed_l.toFixed(2));
    $("#speedr").text(speed_r.toFixed(2));
    $("#resl").text(result_l.toFixed(2));
    $("#resr").text(result_r.toFixed(2));
  }

  $("button").click(function() {
    function toggle(success) {
      if(success) {
        motor = !motor;
        $("button").toggleClass("On");
      }
      if(motor && success) {
        $("button").text("MOTOR ON");
      } else if(!motor && success) {
        $("button").text("MOTOR OFF");
      } else {
        $("button").text("ERROR");
      }
    }

    if(!motor) {
      $.ajax({url: "motor?state=on", timeout: 500}).done(function() { toggle(true); }).fail(function() { toggle(); });
    } else {
      $.ajax({url: "motor?state=off", timeout: 500}).done(function() { toggle(true); }).fail(function() { toggle(); });
    }
  });
});
