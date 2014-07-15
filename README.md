Styler
======

## Overview
This module allows you to load CSS files based on query string parameters. E.g. load embed.css whenever a url has ?styler=embed.

## Installation
- Do the standard module install steps.
- Create a folder in sites/all/libraries called styler.

## Configuration
- Place any stylesheets you want to load with the Styler Module in sites/all/libraries/styler
- Create a styler styler rule on the configuration page.

## Example
Load embed.css whenever a url has ?styler=embed

- Place emebed.css in sites/all/libraries/styler
- Create a Styler rule on the Styler configuration page
- The rule name is whatever you want it to be, e.g. Embed
- The parameter name will be syler
- The parameter value will be embed
- The style sheet will be embed.css