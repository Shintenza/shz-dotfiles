/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>
/* appearance */
static const int horizpadbar        = 2;        /* horizontal padding for statusbar */
static const int vertpadbar         = 7;        /* vertical padding for statusbar */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char font[]            = "Hack Nerd Font:size=10";
static char dmenufont[]       = "Hack Nerd Font:size=10";
static const char *fonts[]          = { font };

static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */

static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 7;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 7;       /* vert inner gap between windows */
static const unsigned int gappoh    = 7;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 7;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */

/* bar status colors */
static char statusbgcolor[] = "#1e222a";
static char statusfgcolor[] = "#bbbbbb";
/* bar info colors */
static char norminfobgcolor[] = "#1e222a";
static char norminfofgcolor[] = "#bbbbbb";
static char selinfobgcolor[] = "#3f495c";
static char selinfofgcolor[] = "#eeeeee";
/* bar tags colors */
static char normtagsbgcolor[] = "#1e222a";
static char normtagsfgcolor[] = "#bbbbbb";
static char seltagsbgcolor[] = "#3f495c";
static char seltagsfgcolor[] = "#eeeeee";
/* border colors */
static char selbordercolor[]        = "#005577";
static char normbordercolor[]       = "#444444";

static char *colors[][3]      = {
	/*                      fg                  bg                  border   */
	[SchemeNorm] =          { statusfgcolor,    statusbgcolor,      normbordercolor },
	[SchemeSel]  =          { selinfofgcolor,   selinfobgcolor,     selbordercolor  },
	[SchemeStatus]  =       { statusfgcolor,    statusbgcolor,      "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
	[SchemeTagsSel]  =      { seltagsfgcolor,   seltagsbgcolor,     "#000000"  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsNorm]  =     { normtagsfgcolor,  normtagsbgcolor,    "#000000"  }, // Tagbar left unselected {text,background,not used but cannot be empty}
	[SchemeInfoSel]  =      { selinfofgcolor,   selinfobgcolor,     "#000000"  }, // infobar middle  selected {text,background,not used but cannot be empty}
	[SchemeInfoNorm]  =     { norminfofgcolor,  norminfobgcolor,    "#000000"  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "", "", "󰍩", "󰘐", "󰓇", "󰌔", ""};

static const char *tagsel[][2][2] = {
	/*  norm                            sel       */
	/*  fg         bg                   fg         bg  */
	{ { "#61afef", normtagsbgcolor }, { "#61afef", seltagsbgcolor } },
	{ { "#e06c75", normtagsbgcolor }, { "#e06c75", seltagsbgcolor } },
	{ { "#caaa6a", normtagsbgcolor }, { "#caaa6a", seltagsbgcolor } },
	{ { "#7ec7a2", normtagsbgcolor }, { "#7ec7a2", seltagsbgcolor } },
	{ { "#1cd560", normtagsbgcolor }, { "#1cd560", seltagsbgcolor } },
	{ { "#16b2e6", normtagsbgcolor }, { "#16b2e6", seltagsbgcolor } },
	{ { "#535760", normtagsbgcolor }, { "#535760", seltagsbgcolor } },
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                    instance  title                 tags mask  isfloating  isterminal  noswallow    monitor */
	{ "Brave-browser",          NULL,     NULL,                 1 << 1,         0,          0,          0,          -1 },
	{ "discord",                NULL,     NULL,                 1 << 2,         0,          0,          0,          -1 },
	{ "neovide",                NULL,     NULL,                 1 << 3,         0,          0,          0,          -1 },
	{ "Spotify",                NULL,     NULL,                 1 << 4,         0,          0,          0,          -1 },
	{ "Alacritty",              NULL,     NULL,                      0,         0,          1,          0,          -1 },
	{ "St",                     NULL,     NULL,                      0,         0,          1,          0,          -1 },
	{ NULL,                     NULL,     "Event Tester",            0,         0,          0,          1,          -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static char dmenuheight[] = "27";
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-h", dmenuheight, "-F", "-p", "Run:", NULL };
static const char *passmenu[] = { "passmenu", "-fn", dmenufont, "-p", "Search:", "-F", "-h", dmenuheight, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
	{ "font",               STRING,  &font },
	{ "dmenufont",          STRING,  &dmenufont },
	{ "borderpx",          	INTEGER, &borderpx },
	{ "snap",          			INTEGER, &snap },
	{ "showbar",          	INTEGER, &showbar },
	{ "topbar",          		INTEGER, &topbar },
	{ "nmaster",          	INTEGER, &nmaster },
	{ "resizehints",       	INTEGER, &resizehints },
	{ "mfact",      	 			FLOAT,   &mfact },
	{ "statusbgcolor",      STRING,  &statusbgcolor },
	{ "statusfgcolor",      STRING,  &statusfgcolor },
	{ "norminfobgcolor",    STRING,  &norminfobgcolor },
	{ "norminfofgcolor",    STRING,  &norminfofgcolor },
	{ "selinfobgcolor",     STRING,  &selinfobgcolor },
	{ "selinfofgcolor",     STRING,  &selinfofgcolor },
	{ "normtagsbgcolor",    STRING,  &normtagsbgcolor },
	{ "normtagsfgcolor",    STRING,  &normtagsfgcolor },
	{ "seltagsbgcolor",     STRING,  &seltagsbgcolor },
	{ "seltagsfgcolor",     STRING,  &seltagsfgcolor },
	{ "selbordercolor",     STRING,  &selbordercolor },
	{ "normbordercolor",    STRING,  &normbordercolor },
};

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_p,      spawn,           {.v = passmenu } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[13]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ControlMask,		    XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ControlMask,           XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {1} }, 

	{ MODKEY|ShiftMask,             XK_q,                       spawn,      SHCMD("sysmenu") },
	{ MODKEY|ShiftMask,             XK_b,                       spawn,      SHCMD("brave --disable-features=UseChromeOSDirectVideoDecoder --enable-features=VaapiVideoDecoder")},
	{ MODKEY|ShiftMask,             XK_f,                       spawn,      SHCMD("alacritty -e ranger")},
	{ MODKEY|ShiftMask,             XK_g,                       spawn,      SHCMD("steam")},
	{ MODKEY|ShiftMask,             XK_m,                       spawn,      SHCMD("geary")},
	{ MODKEY|ShiftMask,             XK_d,                       spawn,      SHCMD("discord")},
	{ MODKEY|ShiftMask,             XK_s,                       spawn,      SHCMD("spotify-launcher")},
	{ MODKEY|ShiftMask,             XK_t,                       spawn,      SHCMD("Thunar")},
	{ MODKEY,             					XK_Print,                   spawn,      SHCMD("maim ~/Pictures/screenshot-$(date +%F_%T).png")}, 
	{ MODKEY|ShiftMask,             XK_Print,                   spawn,      SHCMD("maim -s | xclip -selection clipboard -t image/png")},

	{ 0,             				XF86XK_AudioMute,    								spawn,      SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle; pkill -RTMIN+6 dwmblocks") },
	{ 0,             				XF86XK_AudioLowerVolume,    				spawn,      SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -10%; pkill -RTMIN+6 dwmblocks") },
	{ 0,             				XF86XK_AudioRaiseVolume,    				spawn,      SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +10%; pkill -RTMIN+6 dwmblocks") },
	{ 0,             				XF86XK_MonBrightnessDown,   				spawn,      SHCMD("xbacklight -dec 10; pkill -RTMIN+4 dwmblocks") },
	{ 0,             				XF86XK_MonBrightnessUp,    					spawn,      SHCMD("xbacklight -inc 10; pkill -RTMIN+4 dwmblocks") },
	{ 0,             				XF86XK_AudioPlay ,    							spawn,      SHCMD("playerctl play-pause") },
	{ 0,             				XF86XK_AudioNext,    								spawn,      SHCMD("playerctl next") },
	{ 0,             				XF86XK_AudioPrev,    								spawn,      SHCMD("playerctl previous") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
