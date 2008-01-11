DROP TABLE IF EXISTS `eventai_Scripts`;
CREATE TABLE `eventai_Scripts` (
`id` int(11) unsigned NOT NULL default '0' COMMENT 'Used only for Table sorting',
`creature_id` int(11) unsigned NOT NULL default '0' COMMENT 'Creature Template Identifier',

`event_type` tinyint(5) unsigned NOT NULL default '0' COMMENT 'Event Type',
`event_param1` int(11) unsigned NOT NULL default '0',
`event_param2` int(11) unsigned NOT NULL default '0',
`event_param3` int(11) unsigned NOT NULL default '0',

`action1_type` tinyint(5) unsigned NOT NULL default '0' COMMENT 'Action Type',
`action1_param1` int(11) unsigned NOT NULL default '0',
`action1_param2` int(11) unsigned NOT NULL default '0',
`action1_param3` int(11) unsigned NOT NULL default '0',

`action2_type` tinyint(5) unsigned NOT NULL default '0' COMMENT 'Action Type',
`action2_param1` int(11) unsigned NOT NULL default '0',
`action2_param2` int(11) unsigned NOT NULL default '0',
`action2_param3` int(11) unsigned NOT NULL default '0',

`action3_type` tinyint(5) unsigned NOT NULL default '0' COMMENT 'Action Type',
`action3_param1` int(11) unsigned NOT NULL default '0',
`action3_param2` int(11) unsigned NOT NULL default '0',
`action3_param3` int(11) unsigned NOT NULL default '0',

  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='EventAI Scripts';

DROP TABLE IF EXISTS `localized_texts`;
CREATE TABLE `localized_texts` (
  `id` int(11) unsigned NOT NULL default '0' COMMENT 'Identifier',
`locale_0` varchar(255) NOT NULL default '',
`locale_1` varchar(255) NOT NULL default '',
`locale_2` varchar(255) NOT NULL default '',
`locale_3` varchar(255) NOT NULL default '',
`locale_4` varchar(255) NOT NULL default '',
`locale_5` varchar(255) NOT NULL default '',
`locale_6` varchar(255) NOT NULL default '',
`locale_7` varchar(255) NOT NULL default '',

  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Localized Text';