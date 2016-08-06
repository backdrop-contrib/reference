/**
 * @file
 * Javascript for reference autocomplete field widget.
 */
 (function ($) {

"use strict";

Backdrop.behaviors.reference_autocomplete = {
  attach: function (context) {
    $('.reference-autocomplete-select').change( function() {
      var entity_bundle = $(this).val();
      var autocomplete_id = $(this).attr('id').replace('entity-bundle', 'target-id');
      var autocomplete_url = $('#' + autocomplete_id + "-autocomplete").val();
      var autocomplete_url_base = autocomplete_url.substr(0, autocomplete_url.indexOf('reference_autocomplete'));
      $('#' + autocomplete_id + "-autocomplete").val(autocomplete_url_base + 'reference_autocomplete/' + entity_bundle);
      $('#' + autocomplete_id + "-autocomplete").removeClass('autocomplete-processed');
      $('#' + autocomplete_id).unbind().val('');
      Backdrop.behaviors.autocomplete.attach(document);
    });
  }
};

})(jQuery);