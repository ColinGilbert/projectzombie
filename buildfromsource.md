# Introduction #

A how-to for building from source.

# Details #

**Data dependencies**

The project won't run for you (LOL). I don't have the data uploaded. I suppose you could parse the source and figure out the data that is needed, but why would you waste your time on that. Sooner or later I will upload the data, but that is not yet a priority. I'm not yet at the phase where I'm ready to release this to the public. However, you can make an request, that way I would know that someone actually wants to run this thing, and I will make it a priority then to work with you on it, then.

**Building using Scons**

An initial set of Scons script were written. Please refer to that for details on dependencies (OpenCL, Ogre, boost.)

**MSVC2010 and MSVC2005 project files.**

Project files using visual studio are made available. However, I'm currently developing on Linux using Scons, so the visual studio project files are no longer updated. I will update these project files on some arbitrary interval, though. I suppose one could use MSVC and use their custom build chain option. I would not know as I'm using the Express version where this option is unavailable.

**From Eclipse using CDT**

You can use Eclipse for this project (this is my current development environment) with the custom build chain using scons. I've decided not to include the Eclipse project files in the source, as building the project file is fairly straightforward on your own. Simply start a new project, and invoke scons.
