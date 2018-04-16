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
  for (var i = 0; i < json.meetings.length; i++)
  {
    var m = json.meetings[i];
    data.addRow([m[0], new Date(0, 0, 0, m[1].h, m[1].m, m[1].s),
                       new Date(0, 0, 0, m[2].h, m[2].m, m[2].s)]);
  }

  var chart = new google.visualization.Timeline(document.getElementById('week_review'));
  chart.draw(data, {height: 340});

  google.visualization.events.addListener(chart, 'select', function() {
    var json = getDataFor('week');

    var selection_timestamp = json.meetings[chart.getSelection()[0].row][1].t;
    showMeetingDetailFor(new Date(selection_timestamp*1000.0));
  });
}

function showMeetingDetailFor(date) {
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Section');
  data.addColumn('number', 'Time');

  var json = getDataFor('meeting', date);
  data.addRows(json.sections);

  document.getElementById('meeting_details').innerHTML = json.meeting_info;

  var card_width = $('#meeting_detail_card').width();
  var chart = new google.visualization.PieChart(document.getElementById('meeting_breakdown'));
  chart.draw(data, {pieHole: 0.4, height: card_width, legend: 'none'});
}

function showMeetingDetail() {
  showMeetingDetailFor(new Date());
}

function showTrends() {
  var data = new google.visualization.DataTable();
  data.addColumn('string', 'Date');
  data.addColumn('number', 'Total meeting time');
  data.addColumn('number', 'Number of speakers');
  data.addColumn('number', 'Average speaker time');

  var json = getDataFor('trend');
  data.addRows(json.trends);

  var chart = new google.visualization.LineChart(document.getElementById('trends'));
  chart.draw(data, {legend: {position: 'bottom'}});
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
