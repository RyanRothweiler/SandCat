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

	// Events
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetEventsCount();
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetEventIndexName(int methodIndex, StringBuilder strBuffer);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetEventIndexNameLength(int methodIndex);

	[DllImport ("SandCat_Runner")] private static extern int 	SC_DoesActionExist(string actionName);
	[DllImport ("SandCat_Runner")] private static extern void 	SC_DoAction(string actionName, string paras);

	// Fluents
	[DllImport ("SandCat_Runner")] private static extern float	SC_GetFluentValue(string fluentName);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_DoesFluentExist(string fluentName);

	// Entities
	[DllImport ("SandCat_Runner")] private static extern int 	SC_DoesEntityExist(string entityName);
	[DllImport ("SandCat_Runner")] private static extern float	SC_GetEntityFluent(string entityName, string fluentName);

	// Arrays
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetFluentInArray(string ArrayName, int ArrayIndex, string FluentName);
	[DllImport ("SandCat_Runner")] private static extern bool 	SC_ArrayExists(string arrayName);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetArrayLength(string arrayName);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetArrayHeight(string arrayName);
	[DllImport ("SandCat_Runner")] private static extern int 	SC_GetArrayWidth(string arrayName);

	// Testing
	[DllImport ("SandCat_Runner")] private static extern int 	SC_IntTest(int input);
	[DllImport ("SandCat_Runner")] private static extern void 	SC_GetStatePrintout(StringBuilder printoutBuffer, int bufferSize);


#endif

#if UNITY_WEBGL && !UNITY_EDITOR
	// Linked Methods
	[DllImport ("__Internal")] private static extern void 		SC_LoadGame(string progData, int progCharLength, StringBuilder errorDesc, int errorSize);

	// Events
	[DllImport ("__Internal")] private static extern int 		SC_GetEventsCount();
	[DllImport ("__Internal")] private static extern int 		SC_GetEventIndexName(int methodIndex, StringBuilder strBuffer);
	[DllImport ("__Internal")] private static extern int 		SC_GetEventIndexNameLength(int methodIndex);


	[DllImport ("__Internal")] private static extern int 		SC_DoesActionExist(string actionName);
	[DllImport ("__Internal")] private static extern void 		SC_DoAction(string actionName, string paras);

	// Fluents
	[DllImport ("__Internal")] private static extern float 		SC_GetFluentValue(string fluentName);
	[DllImport ("__Internal")] private static extern int 		SC_DoesFluentExist(string fluentName);

	// Entities
	[DllImport ("__Internal")] private static extern int 		SC_DoesEntityExist(string entityName);
	[DllImport ("__Internal")] private static extern float 		SC_GetEntityFluent(string entityName, string fluentName);

	// Arrays
	[DllImport ("__Internal")] private static extern int 		SC_GetFluentInArray(string arrayName, int arrayIndex, string fluentName);
	[DllImport ("__Internal")] private static extern bool 		SC_ArrayExists(string arrayName);
	[DllImport ("__Internal")] private static extern int	 	SC_GetArrayLength(string arrayName);
	[DllImport ("__Internal")] private static extern int	 	SC_GetArrayHeight(string arrayName);
	[DllImport ("__Internal")] private static extern int	 	SC_GetArrayWidth(string arrayName);


	// Testing
	[DllImport ("__Internal")] private static extern int 		SC_IntTest(int input);
	[DllImport ("__Internal")] private static extern void 		SC_GetStatePrintout(StringBuilder printoutBuffer, int bufferSize);
#endif

	[HideInInspector] public string prevError;
	[HideInInspector] public bool doTest;
	[HideInInspector] public bool showInfo;

	private Vector2 windowScrollPos = new Vector2();

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

		if (Input.GetKeyDown(KeyCode.Tab)) {
			showInfo = !showInfo;
		}
	}

	public int GetEventsCount()
	{
		return (SC_GetEventsCount());
	}

	public string GetEventIndexName(int methodIndex)
	{
		if (methodIndex > SC_GetEventsCount()) {
			Debug.LogError("Method index (" + methodIndex + ") is out of bounds.");
		}

		int nameLength = SC_GetEventIndexNameLength(methodIndex);
		if (nameLength == 123456) {
			Debug.LogError("Method index (" + methodIndex + ") is out of bounds.");
		}

		// TODO fix this, why is the while loop needed?
		bool valid = false;
		StringBuilder sb = new StringBuilder(nameLength);
		while (!valid) {
			sb = new StringBuilder(nameLength);
			SC_GetEventIndexName(methodIndex, sb);
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

	public float GetFluentValue(string fluentName)
	{
		if (SC_DoesFluentExist(fluentName) == 1) {
			return (SC_GetFluentValue(fluentName));
		} else {
			Debug.LogError(fluentName + " doesn't exist.");
			return (0);
		}
	}

	public void DoAction(string actionName, string[] paras)
	{
		if (SC_DoesActionExist(actionName) == 1) {
			string commaParams = "";
			for (int index = 0; index < paras.Length; index++) {
				commaParams += paras[index] + ',';
			}

			SC_DoAction(actionName, commaParams);
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

	public float GetFluentInArray(string arrayName, int arrayIndex, string fluentName, string debug_source = "")
	{
		if (SC_ArrayExists(arrayName)) {

			if (SC_GetArrayLength(arrayName) < arrayIndex || arrayIndex <= 0) {
				Debug.LogError("Array index " + arrayIndex + " into " + arrayName + " is out of bounds on " + debug_source + ".");
			} else {
				// TODO Check if that fluent exists
				return (SC_GetFluentInArray(arrayName, arrayIndex, fluentName));
			}
		} else {
			Debug.LogError("Array name of " + arrayName + " does not exist.");
		}

		return (0.0f);
	}

	public void OnGUI()
	{
		if (showInfo) {
			GUILayout.Window(0, new Rect(0, 0, 300, Screen.height), StateWindow, "Raw State");
		}
	}

	public bool CheckArrayExists(string arrayName)
	{
		if (SC_ArrayExists(arrayName)) {
			return (true);
		} else {
			Debug.LogError("The array name of " + arrayName + " does not exist.");
			return (false);
		}
	}

	public int GetArrayHeight(string arrayName)
	{
		CheckArrayExists(arrayName);
		return (SC_GetArrayHeight(arrayName));
	}

	public int GetArrayLength(string arrayName)
	{
		CheckArrayExists(arrayName);
		return (SC_GetArrayHeight(arrayName));
	}

	public void StateWindow(int windowId)
	{
		windowScrollPos = GUILayout.BeginScrollView(windowScrollPos);

		StringBuilder state = new StringBuilder(5000);
		SC_GetStatePrintout(state, 5000);

		GUILayout.Label(state.ToString());

		GUILayout.EndScrollView();
	}
}