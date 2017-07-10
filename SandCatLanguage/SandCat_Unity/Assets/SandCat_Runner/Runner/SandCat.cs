using UnityEngine;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;

[ExecuteInEditMode]
public class SandCat : MonoBehaviour
{

	public static SandCat instance;

	// Rules Definition
	public TextAsset rulesFile;

#if UNITY_EDITOR
	// Linked Methods
	[DllImport ("SandCat_Runner")] private static extern void 	SC_LoadGame(string progData, int progCharLength, StringBuilder errorDesc, int errorSize);

	// Methods
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetMethodsCount();
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetMethodIndexName(int methodIndex, StringBuilder strBuffer);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetMethodIndexNameLength(int methodIndex);

	// Actions
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetActionsCount();
	[DllImport ("SandCat_Runner")] private static extern void 	SC_GetActionNameFromIndex(int actionIndex, StringBuilder strBuffer);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetActionIndexNameLength(int index);

	[DllImport ("SandCat_Runner")] private static extern int 	SC_DoesActionExist(string actionName);
	[DllImport ("SandCat_Runner")] private static extern void 	SC_DoAction(string actionName, StringBuilder errorBuffer, int errorBufferSize);

	// Fluents
	[DllImport ("SandCat_Runner")] private static extern float	SC_GetFluentValue(string fluentName);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_DoesFluentExist(string fluentName);

	// Entities
	[DllImport ("SandCat_Runner")] private static extern int 	SC_DoesEntityExist(string entityName);
	[DllImport ("SandCat_Runner")] private static extern float	SC_GetEntityFluent(string entityName, string fluentName);

	// Arrays
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetFluentInArray(string ArrayName, int ArrayIndex, string FluentName);
	[DllImport ("SandCat_Runner")] private static extern bool 	SC_ArrayExists(string srrayName);

	// Testing
	[DllImport ("SandCat_Runner")] private static extern int 	SC_IntTest(int input);
#endif

#if UNITY_WEBGL && !UNITY_EDITOR
	// Linked Methods
	[DllImport ("__Internal")] private static extern void 		SC_LoadGame(string progData, int progCharLength, StringBuilder errorDesc, int errorSize);

	// Methods
	[DllImport ("__Internal")] private static extern int 		SC_GetMethodsCount();
	[DllImport ("__Internal")] private static extern int 		SC_GetMethodIndexName(int methodIndex, StringBuilder strBuffer);
	[DllImport ("__Internal")] private static extern int 		SC_GetMethodIndexNameLength(int methodIndex);

	// Actions
	[DllImport ("__Internal")] private static extern int 		SC_GetActionsCount();
	[DllImport ("__Internal")] private static extern void 		SC_GetActionNameFromIndex(int actionIndex, StringBuilder strBuffer);
	[DllImport ("__Internal")] private static extern int 		SC_GetActionIndexNameLength(int index);

	[DllImport ("__Internal")] private static extern int 		SC_DoesActionExist(string actionName);
	[DllImport ("__Internal")] private static extern void 		SC_DoAction(string actionName, StringBuilder errorBuffer, int errorBufferSize);

	// Fluents
	[DllImport ("__Internal")] private static extern float 		SC_GetFluentValue(string fluentName);
	[DllImport ("__Internal")] private static extern int 		SC_DoesFluentExist(string fluentName);

	// Entities
	[DllImport ("__Internal")] private static extern int 		SC_DoesEntityExist(string entityName);
	[DllImport ("__Internal")] private static extern float 		SC_GetEntityFluent(string entityName, string fluentName);

	// Arrays
	[DllImport ("__Internal")] private static extern int 		SC_GetFluentInArray(string ArrayName, int ArrayIndex, string FluentName);
	[DllImport ("__Internal")] private static extern bool 		SC_ArrayExists(string srrayName);

	// Testing
	[DllImport ("__Internal")] private static extern int 		SC_IntTest(int input);
#endif

	[HideInInspector] public string prevError;
	[HideInInspector] public bool doTest;

	public void Awake()
	{
		instance = this;
	}

	void Start ()
	{
		if (doTest) {
			Debug.Log("This should be 4 -> " + SC_IntTest(2));
		} else {
			LoadRules();
		}
	}

	public void Update()
	{
		if (instance == null) {
			instance = this;
		}
	}

	public int GetMethodsCount()
	{
		return (SC_GetMethodsCount());
	}

	public int GetActionsCount()
	{
		return (SC_GetActionsCount());
	}

	public string GetMethodIndexName(int methodIndex)
	{
		if (methodIndex > SC_GetMethodsCount()) {
			Debug.LogError("Method index (" + methodIndex + ") is out of bounds.");
		}

		int nameLength = SC_GetMethodIndexNameLength(methodIndex);
		if (nameLength == 123456) {
			Debug.LogError("Method index (" + methodIndex + ") is out of bounds.");
		}

		// TODO fix this, why is the while loop needed?
		bool valid = false;
		StringBuilder sb = new StringBuilder(nameLength);
		while (!valid) {
			sb = new StringBuilder(nameLength);
			SC_GetMethodIndexName(methodIndex, sb);
			if (sb.Length > 0) {
				valid = true;
			}
		}
		return (sb.ToString());
	}

	public float GetEntityFluentValue(string entityName, string fluentName)
	{
		if (SC_DoesEntityExist(entityName) == 1) {
			// TODO check if the fluent in the entity exists before getting it's value
			return (SC_GetEntityFluent(entityName, fluentName));
		} else {
			Debug.LogError("The entity of " + entityName + " doesn't exist");
			return (0);
		}
	}

	public string GetActionIndexName(int index)
	{
		if (index > GetActionsCount()) {
			Debug.LogError("Action index " + index + "is out of bounds.");
		}

		int nameLength = SC_GetActionIndexNameLength(index);
		if (nameLength == 123456) {
			Debug.LogError("Action index " + index + "is out of bounds.");
		}

		// TODO fix this. I don't know why sometimes we get back a blank name
		bool valid = false;
		StringBuilder sb = new StringBuilder(nameLength);
		while (!valid) {
			sb = new StringBuilder(nameLength);
			SC_GetActionNameFromIndex(index, sb);
			if (sb.Length > 0) {
				valid = true;
			}
		}
		return (sb.ToString());
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
			StringBuilder stringBuilder = new StringBuilder(200);
			SC_DoAction(actionName, stringBuilder, stringBuilder.Length);

			if (stringBuilder.ToString().Length > 0 && stringBuilder.ToString()[0] != 'x') {
				Debug.LogError(stringBuilder.ToString());
			}

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
		if (rulesFile != null) {
			StringBuilder error = new StringBuilder(100);
			SC_LoadGame(rulesFile.text, rulesFile.text.Length, error, 100);

			prevError = error.ToString();
			if (prevError != "x") {
				Debug.Log(prevError);
			}
		}
	}

	public float GetFluentInArray(string arrayName, int arrayIndex, string fluentName)
	{
		if (SC_ArrayExists(arrayName)) {
			// TODO Check if that fluent exists
			// Make sure the array index isn't out of bounds, that crashes things.

			return (SC_GetFluentInArray(arrayName, arrayIndex, fluentName));
		} else {
			Debug.LogError("Array name of " + arrayName + " does not exist.");
			return (1.0f);
		}
	}
}