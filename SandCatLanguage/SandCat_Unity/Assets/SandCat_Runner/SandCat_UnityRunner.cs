using UnityEngine;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;

public class SandCat_UnityRunner : MonoBehaviour
{

	// Rules Definition
	public string pathToRules;
	public TextAsset rulesFile;

	// Linked Methods
	[DllImport ("SandCat_Runner")] private static extern int LoadGame(string progData, int progCharLength);
	[DllImport ("SandCat_Runner")] private static extern int GetMethodsCount();
	[DllImport ("SandCat_Runner")] private static extern float GetFluentValue(string fluentName);
	[DllImport ("SandCat_Runner")] private static extern int DoesFluentExist(string fluentName);


	// Use this for initialization
	void Start ()
	{
		CheckSuccess(LoadGame(rulesFile.text, rulesFile.text.Length));
		Debug.Log(FluentGetValue("Zoidberg"));
	}

	public float FluentGetValue(string fluentName)
	{
		if (DoesFluentExist(fluentName) == 1) {
			return (GetFluentValue(fluentName));
		} else {
			Debug.LogError("That fluent doesn't exist.");
			return (0);
		}
	}

	public void CheckSuccess(int succVal)
	{
		if (succVal != 0) {
			Debug.LogError("Error in dll on line " + succVal);
		}
	}
}
