photoshop-aco2UIColors
======================

tiny c program that reaps the color values from a PhotoShop ACO file and turns them into a list of [UIColor colorWithRed: blue: green: alpha:] color definitions. (HEX and RGB as comments at col 77-85 &amp; 86- if you want to `cut` them.) 

It doesn't use a proper reading library, it simply skips the useless
binary words and grabs the colors.

It'll only work with an ACO in RGB colorspace.

## Example output

Read ACO : Generating UIColors (Hex & RGB Triplets @c77-85 & c86-)

    [UIColor colorWithRed:0.658824 green:0.639216 blue:0.556863 alpha:1.0f], // #a8a38e  rgb(168,163,142) 
    [UIColor colorWithRed:0.513726 green:0.486275 blue:0.403922 alpha:1.0f], // #837c67  rgb(131,124,103) 
    [UIColor colorWithRed:0.913725 green:0.709804 blue:0.254902 alpha:1.0f], // #e9b541  rgb(233,181,65) 
    [UIColor colorWithRed:0.792157 green:0.768627 blue:0.709804 alpha:1.0f], // #cac4b5  rgb(202,196,181) 
    [UIColor colorWithRed:0.901961 green:0.886275 blue:0.847059 alpha:1.0f], // #e6e2d8  rgb(230,226,216) 
    [UIColor colorWithRed:0.254902 green:0.227451 blue:0.180392 alpha:1.0f], // #413a2e  rgb(65,58,46) 
    [UIColor colorWithRed:0.400000 green:0.380392 blue:0.313726 alpha:1.0f], // #666150  rgb(102,97,80) 
    [UIColor colorWithRed:0.560784 green:0.537255 blue:0.458824 alpha:1.0f], // #8f8975  rgb(143,137,117) 

## Usage

    cat example.aco | aco2UIColors
    
That's it.

Feel free to do what ever you like with this, GPL is attached.

Based on original work by Salvatore Sanfilippo Copyright(C) 2006 GNU-GPL
http://www.hping.org/aco2html/
