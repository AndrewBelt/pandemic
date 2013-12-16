
var addLayer = function(name, hue) {
	var img = new Image();
	img.actualSrc = '../output/' + name + '.png';
	img.src = img.actualSrc;
	
	$(img).addClass('layer');
	hue %= 360;
	$(img).css('-webkit-filter', 'hue-rotate(' + hue + 'deg)');
	
	$('.viewport').append(img);
};

var refreshLayers = function() {
	$('.layer').each(function(i, layer) {
		layer.src = layer.actualSrc + '?' + new Date().getTime();
	});
};

$(document).ready(function(){
	addLayer('elevation', 120);
	addLayer('prey-population', 240);
	addLayer('predator-population', 0);
	
	setInterval(refreshLayers, 10000);
});
