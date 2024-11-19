#include <iostream>
#include <Core/Application.h>
#include <Core/Buffer.hpp>

/*
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <Utils/FileWatcher.hpp>
*/

void OnApplicationResize(int width, int height)
{
	std::cout << "width : " << width << ", height : " << height << "\n";
}

void OnApplicationKeyboardPressed(PIX3D::KeyCode keycode)
{
	if (keycode == PIX3D::KeyCode::F)
		std::cout << "F Pressed!\n";
}


int main()
{
	std::cout << "Hello, World!\n";

	/*
	mono_set_assemblies_path("mono/lib");

	auto domain = mono_jit_init("PixScriptingEngine");
	if (domain)
	{
		auto assembly = mono_domain_assembly_open(domain, "Resources/Release/net6.0/TestGame.dll");
		if (assembly)
		{
			auto image = mono_assembly_get_image(assembly);

			MonoClass* TestScript = mono_class_from_name(image, "TestGame", "TestScript");

			// create object
			MonoObject* my_class_instance = mono_object_new(domain, TestScript);
			
			// call constructor
			mono_runtime_object_init(my_class_instance);

			// call start
			MonoMethod* start_method = mono_class_get_method_from_name(TestScript, "Start", 0);

			mono_runtime_invoke(start_method, my_class_instance, nullptr, nullptr);

			// call update
			MonoMethod* updtae_method = mono_class_get_method_from_name(TestScript, "Update", 0);

			mono_runtime_invoke(updtae_method, my_class_instance, nullptr, nullptr);

		}
	}
	*/

	PIX3D::ApplicationSpecs AppSpecs;
	AppSpecs.Width = 800;
	AppSpecs.Height = 600;
	AppSpecs.Title = "PIX3D Enigne";

	PIX3D::Application* App = new PIX3D::Application(AppSpecs);

	App->SetWindowSizeCallBackFunc(OnApplicationResize);
	App->SetKeyboardCallBackFuncPtr(OnApplicationKeyboardPressed);

	App->Run();

	delete App;

	return 0;
}
