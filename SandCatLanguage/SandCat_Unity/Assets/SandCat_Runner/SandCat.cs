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
	[DllImport ("SandCat_Runner")] private static extern int 	SC_LoadGame(string progData, int progCharLength);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetMethodsCount();
	[DllImport ("SandCat_Runner")] private static extern float 	SC_GetFluentValue(string fluentName);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_DoesFluentExist(string fluentName);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_DoesEntityExist(string entityName);
	[DllImport ("SandCat_Runner")] private static extern float 	SC_GetEntityFluent(string entityName, string fluentName);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_DoesActionExist(string actionName);
	[DllImport ("SandCat_Runner")] private static extern void 	SC_DoAction(string actionName);

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
		if (SC_DoesEntityExist(entityName) == 1) {
			// TODO check if the fluent in the entity exists before getting it's value
			return (SC_GetEntityFluent(entityName, fluentName));
		} else {
			Debug.LogError("That entity doesn't exist");
			return (0);
		}
	}

	public float GetFluentValue(string fluentName)
	{
		if (SC_DoesFluentExist(fluentName) == 1) {
			return (SC_GetFluentValue(fluentName));
		} else {
			Debug.LogError(fluentName + " doesn't exist.");
			return (0);
		}
	}

	public void DoAction(string actionName)
	{
		if (SC_DoesActionExist(actionName) == 1) {
			SC_DoAction(actionName);
		} else {
			Debug.LogError("Acton " + actionName + " does not exist.");
		}
	}

	public void ReloadRules()
	{
		LoadRules();
	}

	public void LoadRules()
	{
		int succVal = SC_LoadGame(rulesFile.text, rulesFile.text.Length);
		if (succVal != 0) {
			Debug.LogError("Error in dll on line " + succVal);
		} else {
			Debug.Log("Successfully loaded game.");
		}
	}
}