function clamp(number, min, max) {
  return Math.max(min, Math.min(number, max));
}

$(document).ready(function() {
  gyro.frequency = 10;
  gyro.startTracking(function(o) {
    // clamp(Math.round(o.y * -20) / 100, -1, 1);
    steer_l = clamp(Math.round(o.x *  20) / 100, -1, 1);
    steer_r = clamp(Math.round(o.x * -20) / 100, -1, 1);
    speed_l = clamp(Math.round(o.y * -20) / 100, -1, 1);
    speed_r = clamp(Math.round(o.y * -20) / 100, -1, 1);
    result_l = 0;
    result_r = 0;

    if(steer_l > 0.1) {
      result_l = speed_l;
      result_r = clamp(Math.round(speed_r - steer_r * 100) / 100, -1, 1);
    } else if(steer_r > 0.1) {
      result_l = clamp(Math.round(speed_l - steer_l * 100) / 100, -1, 1);
      result_r = speed_r;
    } else {
      result_l = speed_l;
      result_r = speed_r;
    }

    $("#steerl").text(steer_l);
    $("#steerr").text(steer_r);
    $("#speedl").text(speed_l);
    $("#speedr").text(speed_r);
    $("#resl").text(result_l);
    $("#resr").text(result_r);

    $("#x").text(o.x);
    $("#y").text(o.y);
  });
});
