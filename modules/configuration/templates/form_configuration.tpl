{function name=createRadio}
    {if $v eq "1" || $v eq "0"}
    <label class="radio-inline">
        <input type="radio" name="{$k}" value="1" {if $v eq "1"}checked{/if}>Yes
    </label>
    <label class="radio-inline">
        <input type="radio" name="{$k}" value="0" {if $v eq "0"}checked{/if}>No
    </label>
    {else if $v eq "true" || $v eq "false"}
    <label class="radio-inline">
        <input type="radio" name="{$k}" value="true" {if $v eq "true"}checked{/if}>Yes
    </label>
    <label class="radio-inline">
        <input type="radio" name="{$k}" value="false" {if $v eq "false"}checked{/if}>No
    </label>
    {/if}
{/function}

{function name=createInstrument}
    <select class="form-control" name="{$k}">
        {foreach from=$instruments key=name item=label}
            <option {if $v eq $name}selected{/if} value="{$name}">{$label}</option>
        {/foreach}
    </select>
{/function}

{function name=createEmail}
    <input class="form-control" type="email" name="{$k}" id="{$k}" value="{$v}">
{/function}

{function name=createTextArea}
    <textarea class="form-control" rows="4" name="{$k}">{$v}</textarea>
{/function}

{function name=createText}
    <input type="text" class="form-control" name="{$k}" id="{$k}" value="{$v}">
{/function}

{function name=printConfigItem}
<div class="form-group">
    <label class="col-sm-2 control-label config-name" data-toggle="tooltip" data-placement="right" title="{$node['Description']}">{$node['Label']}</label>
    <div class="col-sm-10">
        {if isset($node['Children']) && $node['Children']}
            Child nodes go here
            {call name=printConfigItem node=$node['Children']}
        {else}
            {call name=printForm node=$node}
        {/if}
        {if $node['AllowMultiple'] == 1}
            <button class="btn btn-success add" id="{$node['ID']}" type="button">
                <span class="glyphicon glyphicon-plus"></span> Add field
            </button>
        {/if}
    </div>
</div>
{/function}

{function name=printForm}
    <div class="config-form-group">
    {if $node['AllowMultiple'] == 1}<div class="input-group entry" id="{$node['ID']}">{/if}
    {foreach from=$node['Value'] key=k item=v}
        {if $node['DataType'] eq 'boolean'}
            {call createRadio k=$k v=$v}
        {elseif $node['DataType'] eq 'instrument'}
            {call createInstrument k=$k v=$v}
        {elseif $node['DataType'] eq 'email'}
            {call createEmail k=$k v=$v}
        {elseif $node['DataType'] eq 'textarea'}
            {call createTextArea k=$k v=$v}
        {else}
            {call createText k=$k v=$v}
        {/if}
        {if $node['AllowMultiple'] == 1}
            <div class="input-group-btn">
                <button class="btn btn-danger btn-remove" id="{$k}" type="button" name="remove-{$k}">
                    <span class="glyphicon glyphicon-remove"></span>&nbsp;
                </button>
            </div>
        {/if}
    {foreachelse}
        {assign var=id value={"add-"|cat:$node['ID']} }
        {if $node['DataType'] eq 'boolean'}
            {call createRadio k=$id}
        {elseif $node['DataType'] eq 'instrument'}
            {call createInstrument k=$id}
        {elseif $node['DataType'] eq 'email'}
            {call createEmail k=$id}
        {elseif $node['DataType'] eq 'textarea'}
            {call createTextArea k=$id}
        {else}
            {call createText k=$id}
        {/if}
        {if $node['AllowMultiple'] == 1}
            <div class="input-group-btn">
                <button class="btn btn-danger btn-remove" id="{$k}" type="button" name="remove-{$k}">
                    <span class="glyphicon glyphicon-remove"></span>&nbsp;
                </button>
            </div>
        {/if}
    {/foreach}
    {if $node['AllowMultiple'] == 1}</div>{/if}
    </div>
{/function}

<p>Please enter the various configuration variables into the fields below. For information on how to configure LORIS, please refer to the Help section and/or the Developer's guide.</p>
<br>

<div class="col-md-3">
<ul class="nav nav-pills nav-stacked" role="tablist" data-tabs="tabs">
    {foreach $parentMenuItems as $menuItem name=tabnav}
        {if $smarty.foreach.tabnav.first}
        <li class="active"><a href="#{$menuItem['Name']}" data-toggle="tab">{$menuItem['Label']}</a></li>
        {else}
        <li><a href="#{$menuItem['Name']}" data-toggle="tab">{$menuItem['Label']}</a></li>
        {/if}
    {/foreach}
</ul>
</div>

<div class="col-md-9">
    <div class="tab-content">
    {foreach $config as $topItem name=configContent}
        {if $smarty.foreach.configContent.first}
        <div class="tab-pane active" id="{$topItem['Name']}">

        {else}
        <div class="tab-pane" id="{$topItem['Name']}">

        {/if}
            <h3>{$topItem['Label']}</h3>
            <p>{$topItem['Description']}</p>
            <hr>
            <form class="form-horizontal" role="form" method="post">
            {foreach $topItem['Children'] as $configChild}
                {call name=printConfigItem node=$configChild}
            {/foreach}
                <div class="form-group">
                    <div class="col-sm-offset-2 col-sm-9 submit-area">
                        <button type="submit" class="btn btn-primary">Submit</button>
                        <button type="reset" class="btn btn-default">Reset</button>
                    </div>
                </div>
            </form>
        </div>
    {/foreach}
    </div>
</div>