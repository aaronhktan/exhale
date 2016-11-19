module.exports = function(minified) {
  var clayConfig = this;
  var _ = minified._;
  var $ = minified.$;
  var HTML = minified.HTML;

  function toggleReminder() {
    if (this.get() !== "0") {
      clayConfig.getItemByMessageKey('reminderHoursStart').enable();
    } else {
      clayConfig.getItemByMessageKey('reminderHoursStart').disable();
    }
  }

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
    var reminderSelect = clayConfig.getItemByMessageKey('reminderHours');
    toggleReminder.call(reminderSelect);
    reminderSelect.on('change', toggleReminder);
  });

};