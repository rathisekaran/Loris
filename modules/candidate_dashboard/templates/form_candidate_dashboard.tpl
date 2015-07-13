<div class="panel panel-default">
    <div class="panel-heading" id="panel-main-heading">
        <h3 class="panel-title">Candidate #{$candID}</h3>
        <span class="pull-right clickable glyphicon glyphicon-chevron-up"></span>
    </div> <!-- closing panel-heading div-->
   <div class="panel-body">
        <ul class="nav nav-pills" role="tablist">
           <li role="presentation" class="active">
               <a href="#candidate_info" aria-controls="candidate_info" role="tab" data-toggle="tab">Candidate Info</a></li>
           <li role="presentation">
               <a href="#proband_info" aria-controls="proband_info" role="tab" data-toggle="tab">Proband Info</a></li>
           <li role="presentation">
               <a href="#family_info" aria-controls="family_info" role="tab" data-toggle="tab">Family Info</a></li>
           <li role="presentation">
               <a href="#participant_status" aria-controls="participant_status"
                  role="tab" data-toggle="tab">Participant Status</a></li>
          <li role="presentation">
               <a href="#consent_info" aria-controls="consent_info" role="tab" data-toggle="tab">Consent Info</a></li>
       </ul>
  <br>
  <!-- Tab panes -->
  <div class="tab-content">
    <div role="tabpanel" class="tab-pane active col-sm-12" id="candidate_info">
     <form class="form-horizontal">
       <div class="col-sm-6">
         <div class="row padding">
            <label class="col-sm-8 control-label" data-toggle="tooltip"
                   data-placement="right">{$form.flagged_caveatemptor.label}</label>
            <div class="col-sm-4 pull-right">
                {$form.flagged_caveatemptor.html}
            </div>
        </div>
        <div class="row padding">
            <label class="col-sm-8">{$form.flagged_reason.label}</label>
            <div class="col-sm-4">
                {$form.flagged_reason.html}
            </div>
        </div>
        <div class="row padding">
            <label class="col-sm-8">{$form.flagged_other_group.label}</label>
            <div class="col-sm-4">
                {$form.flagged_other_group.html}
            </div>
        </div>
         <input class="btn btn-sm btn-primary col-sm-offset-11" name="fire_away" value="Save" type="submit" />
      </div>
      <div role="tabpabel" class="tab-pane active col-sm-6" id="candidate_info">
         <!--h3>History </h3-->
        {foreach from=$elements_list item=element}
            <div class="form-group">
                <label class="col-sm-4 control-label">{$form.$element.label}</label>
                <div class="col-sm-8 form-control">
                    {$form.$element.html}
                </div>
            </div>
        {/foreach}

      </div>
     </form>
    </div><!--Closing candidate info div -->
    <div role="tabpanel" class="tab-pane" id="proband_info">
          <div class="col-sm-6">
              <div class="row padding">
                 <label class="col-sm-4 control-label" data-toggle="tooltip"
                        data-placement="right">{$form.ProbandGender.label}</label>
            <div class="col-sm-8">
                {$form.ProbandGender.html}
            </div>
          </div>
          <div class="row padding">
            <label class="col-sm-4">{$form.ProbandDoB.label}</label>
            <div class="col-sm-8">
                {$form.ProbandDoB.html}
            </div>
          </div>
          <div class="row padding">
                <label class="col-sm-4">{$form.Age_Difference.label}</label>
                <div class="col-sm-8">
                    {$form.Age_Difference.html}
                </div>
           </div>
          <input class="btn btn-sm btn-primary col-sm-offset-11" name="fire_away" value="Save" type="submit" />
       </div>
    </div><!--Closing proband_info div -->
    <div role="tabpanel" class="tab-pane" id="family_info">
        <div class="col-sm-6">
         <div class="row padding">
            <label class="col-sm-4">{$form.FamilyMemberID.label}</label>
            <div class="col-sm-8">
                {$form.FamilyMemberID.html}
            </div>
        </div>
        <div class="row padding">
            <label class="col-sm-4">{$form.relation_type.label}</label>
            <div class="col-sm-8">
                {$form.relation_type.html}
            </div>
        </div>
        <input class="btn btn-sm btn-primary col-sm-offset-11" name="fire_away" value="Save" type="submit" />
      </div>
    </div><!--Closing family_info div -->
    <div role="tabpanel" class="tab-pane" id="participant_status">
   </div>
   <div role="tabpanel" class="tab-pane" id="consent_info">
        <h1>Update Consent info</h1>
   </div>
  </div>
   </div>
</div>
