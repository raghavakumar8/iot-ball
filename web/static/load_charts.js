google.charts.load('current', {packages: ['corechart']});
google.charts.setOnLoadCallback(showTrends);

function showTrends() {
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Date');
  data.addColumn('number', 'Depth');

  var json = getDataFor('trend');
  data.addRows(json.data);

  var options = {
    title: 'Some Trend',
    width: 900,
    height: 500
  };

  var chart = new google.visualization.LineChart(document.getElementById('trend'));
  chart.draw(data, options);
}

function getDataFor(chart_type) {
  return $.ajax({
    url: "/chart_data",
    data: {form: {type: chart_type}},
    type: 'POST',
    async: false
  }).responseJSON;
}