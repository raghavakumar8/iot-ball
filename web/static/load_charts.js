google.charts.load('current', {packages: ['corechart', 'timeline']});

google.charts.setOnLoadCallback(showMeetingDetail);
google.charts.setOnLoadCallback(showWeekReview);
google.charts.setOnLoadCallback(showTrends);

function showWeekReview() {
  var data = new google.visualization.DataTable();
  data.addColumn({ type: 'string', id: 'Day' });
  data.addColumn({ type: 'date', id: 'Start' });
  data.addColumn({ type: 'date', id: 'End' });

  var json = getDataFor('week');

  data.addRows([
    [ 'Monday', new Date(1789, 3, 30), new Date(1797, 2, 4) ],
    [ 'Monday', new Date(1797, 2, 4), new Date(1801, 2, 4) ],
    [ 'Monday', new Date(1801, 2, 4), new Date(1809, 2, 4) ],
    [ 'Tuesday', new Date(1789, 3, 21), new Date(1797, 2, 4)],
    [ 'Tuesday', new Date(1797, 2, 4), new Date(1801, 2, 4)],
    [ 'Tuesday', new Date(1801, 2, 4), new Date(1805, 2, 4)],
    [ 'Tuesday', new Date(1805, 2, 4), new Date(1812, 3, 20)],
    [ 'Wednesday', new Date(1789, 8, 25), new Date(1790, 2, 22)],
    [ 'Wednesday', new Date(1790, 2, 22), new Date(1793, 11, 31)],
    [ 'Wednesday', new Date(1794, 0, 2), new Date(1795, 7, 20)],
    [ 'Thursday', new Date(1794, 0, 2), new Date(1794, 0, 2)],
    [ 'Friday', new Date(1795, 7, 20), new Date(1800, 4, 12)],
    [ 'Friday', new Date(1800, 4, 13), new Date(1800, 5, 5)],
    [ 'Saturday', new Date(1800, 5, 13), new Date(1801, 2, 4)],
    [ 'Sunday', new Date(1801, 2, 5), new Date(1801, 4, 1)],
    [ 'Sunday', new Date(1801, 4, 2), new Date(1809, 2, 3)]
  ]);

  var chart = new google.visualization.Timeline(document.getElementById('week_review'));
  chart.draw(data, {height: 340});
}

function showMeetingDetail() {
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Section');
  data.addColumn('number', 'Time');

  var json = getDataFor('meeting');
  data.addRows(json.sections);

  document.getElementById('meeting_details').innerHTML = json.meeting_info;

  var card_width = $('#meeting_detail_card').width();
  var chart = new google.visualization.PieChart(document.getElementById('meeting_breakdown'));
  chart.draw(data, {pieHole: 0.4, height: card_width, legend: 'none'});
}

function showTrends() {
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Date');
  data.addColumn('number', 'Total Scrum Time');

  var json = getDataFor('trend');
  data.addRows(json.data);

  var chart = new google.visualization.LineChart(document.getElementById('trends'));
  chart.draw(data);
}

function getDataFor(chart_type, date) {
  date = date || new Date();

  var timestamp = Math.floor(date.getTime()/1000);

  return $.ajax({
    url: "/chart_data",
    data: {type: chart_type, time: timestamp},
    async: false
  }).responseJSON;
}

$(window).resize(function(){
  showMeetingDetail();
  showWeekReview();
  showTrends();
});
