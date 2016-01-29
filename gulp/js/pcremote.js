$(document).ready(function() {
  motor = false;
  arduinoip = "";
  last = 0;

  $(document).keydown(function(e) {
    function sendpost(left, right) {
      $("#resl").text(left);
      $("#resr").text(right);
      $.ajax({url: "motor?l=" + left + "&r=" + right})
        .done(function() {
          $("#state").text("success.");
        })
        .fail(function() {
          $("#state").text("failed.");
        });
      last = e.which;
    }

    if(e.which == 38 && last != 38) {      // up
      sendpost(1, 1);
      console.log("up");
    }
    else if(e.which == 40 && last != 40) { // down
      sendpost(-1,-1);
      console.log("down");
    }
    else if(e.which == 37 && last != 37) { // left
      sendpost(0, 1);
      console.log("left");
    }
    else if(e.which == 39 && last != 39) { // right
      sendpost(1, 0);
      console.log("right");
    }
    else if(e.which == 32 && last != 32) { // space
      sendpost(0, 0);
      console.log("halt");
    }
  });
});
