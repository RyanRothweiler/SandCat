using UnityEngine;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;

public class SandCat : MonoBehaviour
{

	public static SandCat instance;

	// Rules Definition
	public TextAsset rulesFile;

	// Linked Methods
	[DllImport ("SandCat_Runner")] private static extern int 	LoadGame(string progData, int progCharLength);
	[DllImport ("SandCat_Runner")] private static extern int 	GetMethodsCount();
	[DllImport ("SandCat_Runner")] private static extern float 	GetFluentValue(string fluentName);
	[DllImport ("SandCat_Runner")] private static extern int 	DoesFluentExist(string fluentName);
	[DllImport ("SandCat_Runner")] private static extern int 	DoesEntityExist(string entityName);
	[DllImport ("SandCat_Runner")] private static extern float 	GetEntityFluent(string entityName, string fluentName);

	public void Awake ()
	{
		instance = this;
	}

	void Start ()
	{
		LoadRules();
	}

	public float GetEntityFluentValue(string entityName, string fluentName)
	{
		if (DoesEntityExist(entityName) == 1) {
			// TODO check if the fluent in the entity exists before getting it's value
			return (GetEntityFluent(entityName, fluentName));
		} else {
			Debug.LogError("That entity doesn't exist");
			return (0);
		}
	}

	public float FluentGetValue(string fluentName)
	{
		if (DoesFluentExist(fluentName) == 1) {
			return (GetFluentValue(fluentName));
		} else {
			Debug.LogError(fluentName + " doesn't exist.");
			return (0);
		}
	}

	public void ReloadRules()
	{
		LoadRules();
	}

	public void LoadRules()
	{
		int succVal = LoadGame(rulesFile.text, rulesFile.text.Length);
		if (succVal != 0) {
			Debug.LogError("Error in dll on line " + succVal);
		} else {
			Debug.Log("Successfully loaded game.");
		}
	}
}