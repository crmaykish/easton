var map;

$(document).ready(function(){
    var socket = io.connect();

    var map = L.map('map', {
        zoomControl: false,
        dragging: false,
        touchZoom: false,
        scrollWheelZoom: false,
        doubleClickZoom: false,
        boxZoom: false,
        keyboard: false
    });

    map.setZoom(18);

    L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png').addTo(map);

    var LeafIcon = L.Icon.extend({
        options: {
            iconSize:   [32, 32],
            iconAnchor: [16, 16]
        }
    });

    var icon = new LeafIcon({iconUrl: "/static/images/icon.png"});

    var centerMarker = L.marker([0,0], {icon: icon});
    centerMarker.addTo(map);

    socket.on('gps', function(message) {
        console.log(message);
        $('#time').html("Time (UTC): " + message.rmc.time);
        $('#coords').html(message.rmc.latitude.toFixed(6) + ', ' + message.rmc.longitude.toFixed(6));
        $('#speed').html("Speed (Knots): " + message.rmc.speed);
        $('#altitude').html("Altitude (M): " + message.gga.altitude);
        $('#satellites').html("Satellites: " + message.gga.satellites);
        $('#fix').html("Fix Quality: " + message.gga.fix_quality);
        map.setView([message.rmc.latitude, message.rmc.longitude]);
        centerMarker.setLatLng([message.rmc.latitude, message.rmc.longitude]);
    });

    socket.on('motor', function(message) {
        console.log(message);
        $('#motor').html("Motor Speed (L, R): " + message.l_speed + ", " + message.r_speed);
        $('#compass').html("Heading: " + message.heading);
        centerMarker.setRotationAngle(message.heading);
    });

    $("#go").click(function() {
        socket.emit('drive');
    });

    $("#stop").click(function() {
        socket.emit('stop');
    });

});
