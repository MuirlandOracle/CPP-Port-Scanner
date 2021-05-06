# C++ Port Scanner
## Multi-threaded C++ Port Scanner. Originally designed for Abertay CMP202 (DS&Alg2)

### Depends on:
* https://github.com/CLIUtils/CLI11
* https://github.com/SFML/SFML


### Configuring SFML:
* Download [Library](https://www.sfml-dev.org/download/sfml/2.5.1) and extract into a subdirectory
* In Project Properties:
	* C/C++ -> General -> Additional Include Directories. Add `PATH/TO/SFML-2.5.1/include`.
	* Linker -> General -> Additional Library Directories. Add `PATH/TO/SFML-2.5.1/lib`.
	* Linker -> Input -> Additional Dependencies. 
		* For Debug mode: Add `sfml-network-s-d.lib;sfml-system-s-d.lib;ws2_32.lib;winmm.lib`
		* For Release mode: Add `sfml-network-s.lib;sfml-system-s.lib;ws2_32.lib;winmm.lib`
	* C/C++ -> Preprocessor -> Preprocessor Definitions. Add `SFML_STATIC`
